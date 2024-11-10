#include "Timer.h"
#include <chrono>

bool Timer::instantiated = false;

Timer::Timer()
{
	if (instantiated)
	{
		throw;
	}
	instantiated = true;
	deltaTime = 0;
	sumTime = 0;
}

Timer::~Timer()
{
}

void Timer::Update()
{
	double lastDeltaTime = deltaTime;
	
	previousTime = currentTime;
	currentTime = std::chrono::high_resolution_clock::now();

	deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - previousTime).count() / 1000000.0;

	//Iron out massive leaps in deltaTime
	if (deltaTime > lastDeltaTime + 0.25f)
	{
		deltaTime = lastDeltaTime;
	}
	//Add delta time to the sum time
	sumTime += deltaTime;
}
