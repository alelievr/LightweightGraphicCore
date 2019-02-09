#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include "Core/Application.hpp"


namespace LWGC
{
	using NanoSecondTime = std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>;
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

			static float	GetDeltaTime(void);
			static float	GetUnscaledDeltaTime(void);
			static float	GetTime(void);
			static float	GetUnscaledTime();
			static float	GetTimeScale(void);
			static int		GetFrameCount(void);

			Time &	operator=(Time const & src) = delete;
	};
}
