#pragma once

#include "IncludeDeps.hpp"
#include "Core/Object.hpp"
#include "Core/GameObject.hpp"

#include <iostream>
#include <string>

namespace LWGC
{
	class		Template : public Object, public Component
	{
		private:

		public:
			Template(void);
			Template(const Template &) = delete;
			virtual ~Template(void);

			Template &	operator=(Template const & src) = delete;

			virtual void OnAdded(GameObject &go) noexcept override;
			virtual void OnRemoved(const GameObject & go) noexcept override;

			virtual void OnEnable(void) noexcept override;
			virtual void OnDisable(void) noexcept override;

			virtual void Initialize(void) noexcept override;

			virtual uint32_t	GetType(void) const noexcept override;
	};

	std::ostream &	operator<<(std::ostream & o, Template const & r);
}
