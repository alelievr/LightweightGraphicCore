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
			static int 				_frameCount;

		public:
			Time() = delete;
			Time(const Time&) = delete;
			virtual ~Time(void)  = delete;
			
			static void		FrameCount(void);
			
			static void		SetStartTime(void);
			static void		SetScale(float scale);

			static double	GetDeltaTime(void);
			static double	GetUnscaledDeltaTime();
			static float	GetTimeScale(void);
			static int		GetFrameCount(void);

			Time &	operator=(Time const & src) = delete;
	};
}

#endif
