#pragma once
#include <chrono>


class Timer
{
public:
	Timer();
	~Timer();

	bool Update();
	//Set the maximum framerate
	void setMaxFrameRate(double maxFrameRate);

	double getDeltaTime() { return deltaTime; }
	double getSumTime() { return sumTime; }
protected:
	static bool instantiated;

	double deltaTime;
	double sumTime;

	double maxTimeDiff;

	std::chrono::steady_clock::time_point previousTime;
	std::chrono::steady_clock::time_point currentTime;
};