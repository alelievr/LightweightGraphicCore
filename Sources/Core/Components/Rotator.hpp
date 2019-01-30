#pragma once

#include "IncludeDeps.hpp"
#include "Core/Object.hpp"
#include "Core/GameObject.hpp"

#include <iostream>
#include <string>

namespace LWGC
{
	class		Rotator : public Object, public Component
	{
		private:

		public:
			Rotator(void) = default;
			Rotator(const Rotator &) = delete;
			virtual ~Rotator(void) = default;

			Rotator &	operator=(Rotator const & src) = delete;

			virtual void Update(void) noexcept override;

			virtual uint32_t	GetType(void) const noexcept override;
	};

	std::ostream &	operator<<(std::ostream & o, Rotator const & r);
}
