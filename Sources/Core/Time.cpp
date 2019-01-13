#include "Time.hpp"

using namespace LWGC;

NanoSecondTime		Time::_start;
NanoSecondTime		Time::_tmp;
float 				scale = 1;
double				_value = 0;

void Time::SetStartTime(void)
{
	_start = std::chrono::high_resolution_clock::now();	
}

double	Time::GetUnscaledDeltaTime(void)
{
	auto now = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> elapsed_seconds = now - _start;

	std::cout << "elapsed time: " << elapsed_seconds.count() << std::endl;
	
	return (elapsed_seconds.count());
}

