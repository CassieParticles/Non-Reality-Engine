#pragma once
#include <chrono>


class Timer
{
public:
	Timer();
	~Timer();

	void Update();

	double getDeltaTime() { return deltaTime; }
	double getSumTime() { return sumTime; }
protected:
	static bool instantiated;

	double deltaTime;
	double sumTime;

	std::chrono::steady_clock::time_point previousTime;
	std::chrono::steady_clock::time_point currentTime;
};