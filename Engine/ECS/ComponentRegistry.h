#pragma once

#include <vector>
#include<unordered_map>
#include <new>

#include "ECSDefinitions.h"
#include "Component.h"

class Registry;

class IComponentRegistry
{
public:
	//Remove component from registry, overwrite it's data with component at end of array to maintain contiguousness
	virtual void RemoveComponent(EntityId entId) = 0;
	//Return if the component exists from entityId
	virtual bool HasComponent(EntityId entId) = 0;
	//Get the component attached to entId
	virtual BaseComponent* GetComponent(EntityId entId) = 0;
	//Copy component C attached to entity entId
	virtual void CopyComponent(EntityId origEntId,EntityId newEntId) = 0;
protected:
	virtual void resize() = 0;

	//Set up the component to be attached to the entity
	void createComponent(EntityId entId, BaseComponent* base)
	{
		base->entityId = entId;
		base->OnComponentAdded();
	}
	//Set up the component to be attached to the entity
	void copyComponent(EntityId entId, BaseComponent* base)
	{
		base->entityId = entId;
		base->OnComponentCopied();
	}

	//Call OnComponentDestroyed (Interface is a friend function)
	void removeComponent(BaseComponent* base)
	{
		base->OnComponentRemoved();
	}
};

template<Component C>
class ComponentRegistry final : public IComponentRegistry
{
	friend class Registry;
public:
	template<typename... Args>
	//Add component C to entity entId, return a pointer to the component
	C* AddComponent(EntityId entId, Args... args);	
	//Remove component C from entity entId
	void RemoveComponent(EntityId entId) override;
	//Checks whether component C is in entity entId
	bool HasComponent(EntityId entId) override;
	//Get BaseComponent ptr to component C in entity entId
	BaseComponent* GetComponent(EntityId entId) override;
	//Copy component C attached to entity entId
	void CopyComponent(EntityId origEntId, EntityId newEntId) override;
private:
	ComponentRegistry() { componentVector.resize(256); }	//Private constructor, ComponentRegistry can only be created by registry
	std::vector<uint8_t> componentVector;	//Byte array for storing components, using bytes ensures component default constructor isn't called
	int nextFreeIndex{};

	std::unordered_map<EntityId, size_t> EntIndexMap;	//Maps EntityId to the index that entities component is stored at

	void resize() override;
};



template<Component C>
template<typename ...Args>
inline C* ComponentRegistry<C>::AddComponent(EntityId entId, Args ...args)
{
	//Ensure duplicate component is not created
	if (HasComponent(entId)) 
	{
		return reinterpret_cast<C*>(GetComponent(entId)); 
	}
	if ((nextFreeIndex + 1) * sizeof(C) >= componentVector.size())
	{
		resize();
	}
	
	//Create new component
	C* nextFreePtr = reinterpret_cast<C*>(componentVector.data() + nextFreeIndex * sizeof(C));
	new(nextFreePtr) C(std::forward<Args>(args)...);

	//Insert into map
	EntIndexMap.insert({ entId, nextFreeIndex });

	//Set EntityId within component
	BaseComponent* baseComp = reinterpret_cast<BaseComponent*>(nextFreePtr);
	createComponent(entId, baseComp);

	++nextFreeIndex;
	return nextFreePtr;
}

template<Component C>
inline void ComponentRegistry<C>::RemoveComponent(EntityId entId)
{
	if (!HasComponent(entId)) { return; }

	//Get a pointer to the component being removed
	int indexRemoved = EntIndexMap.at(entId);
	C* ptrRemovedComp = reinterpret_cast<C*>(componentVector.data() + indexRemoved * sizeof(C));

	//Remove and destroy component
	BaseComponent* rmBaseComp = reinterpret_cast<BaseComponent*>(ptrRemovedComp);
	removeComponent(rmBaseComp);
	ptrRemovedComp->~C();

	//Copy component at end of array to memory location of removed component
	int lastIndex = nextFreeIndex - 1;
	C* ptrLastComp = reinterpret_cast<C*>(componentVector.data() + lastIndex * sizeof(C));
	memcpy(ptrRemovedComp, ptrLastComp, sizeof(C));

	//Update map to reflect changes
	EntIndexMap.erase(entId);

	if (indexRemoved != lastIndex)
	{
		EntIndexMap.at(ptrLastComp->getEntityId()) = indexRemoved;
	}

	//Decrement next free index
	--nextFreeIndex;
}

template<Component C>
inline bool ComponentRegistry<C>::HasComponent(EntityId entId)
{
	return EntIndexMap.contains(entId);
}

template<Component C>
inline BaseComponent* ComponentRegistry<C>::GetComponent(EntityId entId)
{
	if (!HasComponent(entId)) { return nullptr; }

	int index = EntIndexMap.at(entId);
	BaseComponent* baseCompPtr = reinterpret_cast<BaseComponent*>(componentVector.data() + index * sizeof(C));

	return baseCompPtr;
}

template<Component C>
inline void ComponentRegistry<C>::CopyComponent(EntityId origEntId, EntityId newEntId)
{
	ZoneScopedN("Copy component");
	//Checks to ensure space for new component exists, and component exists on old game object
	if (!HasComponent(origEntId))
	{
		return;
	}
	if ((nextFreeIndex + 1) * sizeof(C) >= componentVector.size())
	{
		resize();
	}

	//Get component to be copied
	BaseComponent* baseComp = GetComponent(origEntId);
	C* origComp = reinterpret_cast<C*>(baseComp);
	//Copy component and get pointer to it
	C* newComp = reinterpret_cast<C*>(componentVector.data() + nextFreeIndex * sizeof(C));
	new(newComp) C(*origComp);
	//Set up the copied component
	BaseComponent* newCompBase = reinterpret_cast<BaseComponent*>(newComp);
	copyComponent(newEntId, newCompBase);

	//Update registry map with new component
	EntIndexMap.insert({ newEntId,nextFreeIndex });

	++nextFreeIndex;
}

template<Component C>
inline void ComponentRegistry<C>::resize()
{
	ZoneScopedN("Resize component registry");
	componentVector.resize(componentVector.size() * 2);
}
