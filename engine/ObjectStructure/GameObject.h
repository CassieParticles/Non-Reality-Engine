#pragma once

//Specific component within the transform component
#include "TransformComponent.h"

#include "DataComponent.h"
#include "InputComponent.h"
#include "UpdateComponent.h"
#include "RenderComponent.h"


class GameObject
{
public:

protected:
	TransformComponent transformComponent;
};