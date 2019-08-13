#include "Timer.h"

Timer::Timer()
{
	sceRtcGetCurrentTick(&timeLast);
	tickResolution = sceRtcGetTickResolution();
}

Timer::~Timer()
{

}

float Timer::deltaTime(void)
{
	sceRtcGetCurrentTick(&timeCurrent);
	float dt = (timeCurrent - timeLast) / ((float)tickResolution);
	timeLast = timeCurrent;

	return dt;
}
