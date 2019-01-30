#pragma once

#include "IncludeDeps.hpp"
#include "Core/Object.hpp"
#include "Core/GameObject.hpp"

#include <iostream>
#include <string>

namespace LWGC
{
	class		Activator : public Object, public Component
	{
		private:
			void ForceUpdate(void) noexcept;

		public:
			Activator(void) = default;
			Activator(const Activator &) = delete;
			virtual ~Activator(void) = default;

			Activator &	operator=(Activator const & src) = delete;

			void Initialize(void) noexcept override;

			virtual uint32_t	GetType(void) const noexcept override;
	};

	std::ostream &	operator<<(std::ostream & o, Activator const & r);
}
