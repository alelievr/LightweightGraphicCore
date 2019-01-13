#include "Time.hpp"

using namespace LWGC;

NanoSecondTime		Time::_start;
NanoSecondTime		Time::_tmp;
float 				Time::_scale = 1;
double				Time::_value = 0;
int 				Time::_frameCount = 0;

void	Time::FrameCount(void)
{
	_frameCount++;
}

void	Time::SetStartTime(void)
{
	_start = std::chrono::high_resolution_clock::now();
}

void	Time::SetScale(float scale)
{
	_scale = scale;

	auto now = std::chrono::high_resolution_clock::now();

	if (_value == 0)
	{
		_value = (now - _start).count() * scale;
		_tmp = std::chrono::high_resolution_clock::now();
	}
	else
	{
		_value += (now - _tmp).count() * scale;
		_tmp = std::chrono::high_resolution_clock::now();
	}
}

double	Time::GetTime(void)
{
	auto now = std::chrono::high_resolution_clock::now();
	
	if (_value == 0)
		return(_value + (now - _start).count() * _scale);
	else
		return(_value  + (now - _tmp).count() * _scale);
}

double	Time::GetUnscaledTime(void)
{
	auto now = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> elapsed_seconds = now - _start;

	return (elapsed_seconds.count());
}

float	Time::GetTimeScale(void)
{
	return _scale;
}

int		Time::GetFrameCount(void)
{
	return _frameCount;
}