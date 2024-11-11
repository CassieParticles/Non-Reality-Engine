#include "Timer.h"
#include "Timer.h"
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
	setMaxFrameRate(-1);

	currentTime = std::chrono::high_resolution_clock::now();
	previousTime = std::chrono::high_resolution_clock::now();
}

Timer::~Timer()
{
}

bool Timer::Update()
{

	currentTime = std::chrono::high_resolution_clock::now();
	double localDT = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - previousTime).count() / 1000000000.0;

	if ( localDT > maxTimeDiff)
	{
		deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - previousTime).count() / 1000000000.0;

		previousTime = currentTime;

		sumTime += deltaTime;

		return true;
	}
	return false;
	
	//previousTime = currentTime;
	//currentTime = std::chrono::high_resolution_clock::now();

	//deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - previousTime).count() / 1000000000.0;

	////Iron out massive leaps in deltaTime
	//if (deltaTime > lastDeltaTime + 0.25f)
	//{
	//	deltaTime = lastDeltaTime;
	//}
	////Add delta time to the sum time

}


void Timer::setMaxFrameRate(double maxFrameRate)
{
	maxTimeDiff = 1.0 / maxFrameRate;
}
