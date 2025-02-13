#pragma once

#include "ECS/ECS.h"
#include <iostream>

class TestCompA : public BaseComponent
{
public:
	void OnComponentAdded() override { std::cout << "Test component added\n"; }
	void OnComponentRemoved() override { std::cout << "Test component removed\n"; }
};