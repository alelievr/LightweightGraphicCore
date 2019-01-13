#ifndef TIME_HPP
# define TIME_HPP
# include <iostream>
# include <string>
#include <chrono>
#include <ctime>

namespace LWGC
{
	using NanoSecondTime = std::__1::chrono::time_point<std::__1::chrono::steady_clock, std::__1::chrono::nanoseconds>;
	
	class		Time
	{
		private:
			static NanoSecondTime	_start;
			static NanoSecondTime	_tmp;
			static double			_value;
			static float 			_scale;
			// static std::chrono::duration<double> _unscaledDeltaTime


		public:
			Time() = delete;
			Time(const Time&) = delete;
			virtual ~Time(void)  = delete;
			
			static void		SetStartTime();
			
			// GetTime();
			// GetDeltaTime()
			// GetTimeScale()
			
			static double	GetUnscaledDeltaTime();
			// GetUnscaleDeltaTime()
			// GetFrameCount()

			Time &	operator=(Time const & src) = delete;
	};
}

#endif
