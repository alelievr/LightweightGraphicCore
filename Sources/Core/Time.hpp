#ifndef TIME_HPP
# define TIME_HPP
# include <iostream>
# include <string>
#include <chrono>
#include <ctime>
#include "Core/Application.hpp"


namespace LWGC
{
	using NanoSecondTime = std::__1::chrono::time_point<std::__1::chrono::steady_clock, std::__1::chrono::nanoseconds>;
	class		Time
	{
		friend class Application;
		
		private:
			static NanoSecondTime	_start;
			static NanoSecondTime	_diff;
			static NanoSecondTime	_lastdeltaTime;
			static double			_value;
			static double			_deltaTime;
			static double 			_scale;
			static int 				_frameCount;
			
			static void		Initialize(void);

		public:
			Time() = delete;
			Time(const Time&) = delete;
			virtual ~Time(void)  = delete;
			
			static void		BeginFrame(void);
			
			static void		SetScale(float scale);

			static double	GetDeltaTime(void);
			static double	GetUnscaledDeltaTime(void);
			static double	GetTime(void);
			static double	GetUnscaledTime();
			static float	GetTimeScale(void);
			static int		GetFrameCount(void);

			Time &	operator=(Time const & src) = delete;
	};
}

#endif
