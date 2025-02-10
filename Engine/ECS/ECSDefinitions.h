#pragma once
#include <cstdint>
#include <type_traits>

typedef uint32_t EntityId;

class BaseComponent;
template<typename T>
concept Component = std::is_base_of<BaseComponent,T>::value;
