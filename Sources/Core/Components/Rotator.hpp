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
			Rotator(void);
			Rotator(const Rotator &) = delete;
			virtual ~Rotator(void);

			Rotator &	operator=(Rotator const & src) = delete;

			virtual void OnAdded(const GameObject &go) noexcept override;
			virtual void OnRemoved(const GameObject & go) noexcept override;
			
			virtual void OnEnable(void) noexcept override;
			virtual void OnDisable(void) noexcept override;
			virtual void Update(void) noexcept override;

			virtual void Initialize(void) noexcept override;

			virtual uint32_t	GetType(void) const noexcept override;
	};

	std::ostream &	operator<<(std::ostream & o, Rotator const & r);
}
