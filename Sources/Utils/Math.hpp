#pragma once

#include <iostream>
#include <string>
#include <cmath>

class		Math
{
	private:


	public:
		Math(void) = delete;
		Math(const Math &) = delete;
		virtual ~Math(void);

		static float		DegToRad;
		static float		RadToDeg;

		Math &	operator=(Math const & src) = delete;
};
