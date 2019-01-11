#pragma once

#include <iostream>
#include <string>

namespace LWGC
{
	enum class	BuiltinTools
	{
		Position,
		Rotation,
		Scale,
	};

	class		Tools
	{
		private:
			static int		_toolIndex;

			static void		Update(void) noexcept;

		public:
			Tools(void) = delete;
			Tools(const Tools&) = delete;
			virtual ~Tools(void) = delete;

			static void		Initialize(void) noexcept;
			static void		DrawTools(void) noexcept;

			static int		GetCurrentToolIndex(void);

			Tools &	operator=(Tools const & src) = delete;
	};
}