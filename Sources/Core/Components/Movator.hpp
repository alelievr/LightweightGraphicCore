#pragma once

#include "IncludeDeps.hpp"
#include "Core/Object.hpp"
#include "Core/GameObject.hpp"

#include <iostream>
#include <string>

namespace LWGC
{
	class		Movator : public Object, public Component
	{
		private:

		public:
			Movator(void) = default;
			Movator(const Movator &) = delete;
			virtual ~Movator(void) = default;

			Movator &	operator=(Movator const & src) = delete;

			void Update(void) noexcept override;

			virtual uint32_t	GetType(void) const noexcept override;
	};

	std::ostream &	operator<<(std::ostream & o, Movator const & r);
}
