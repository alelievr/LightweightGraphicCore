#include "Time.hpp"

using namespace LWGC;

NanoSecondTime		Time::_start;
NanoSecondTime		Time::_diff;
NanoSecondTime		Time::_lastdeltaTime = std::chrono::high_resolution_clock::now();
double 				Time::_scale = 1.0;
double				Time::_value = 0.0;
double				Time::_deltaTime = -1.0;
int 				Time::_frameCount = 0;

void	Time::FrameCount(void)
{
	auto now = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> elapsed_seconds = now - _lastdeltaTime;
	_deltaTime = elapsed_seconds.count();
	_lastdeltaTime = std::chrono::high_resolution_clock::now();
	
	_frameCount++;
}

void	Time::SetStartTime(void)
{
	_start = std::chrono::high_resolution_clock::now();
	_diff = std::chrono::high_resolution_clock::now();
}

void	Time::SetScale(float scale)
{
	auto now = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> elapsed_seconds = now - _diff;
	_value += elapsed_seconds.count() * _scale;
	_diff = std::chrono::high_resolution_clock::now();
	
	_scale = scale;
}

double	Time::GetDeltaTime(void)
{
	return _deltaTime * _scale;
}

double	Time::GetUnscaledDeltaTime(void)
{
	return _deltaTime;
}

double	Time::GetTime(void)
{
	auto now = std::chrono::high_resolution_clock::now();
	
	std::chrono::duration<double> elapsed_seconds = now - _diff;

	return _value + (elapsed_seconds.count() * _scale);
}

double	Time::GetUnscaledTime(void)
{
	auto now = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> elapsed_seconds = now - _start;

	return elapsed_seconds.count();
}

float	Time::GetTimeScale(void)
{
	return _scale;
}

int		Time::GetFrameCount(void)
{
	return _frameCount;
}