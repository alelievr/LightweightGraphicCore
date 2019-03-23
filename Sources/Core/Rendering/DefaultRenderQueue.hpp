#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "Core/Components/Renderer.hpp"
#include "Core/Rendering/IRenderQueue.hpp"
#include "Core/Components/Camera.hpp"

namespace LWGC
{
	class		DefaultRenderQueue : public IRenderQueue
	{
		private:
			std::vector< Renderer * >	_opaqueObjects;
			std::vector< Renderer * >	_transparentObjects;

			void		SortRenderQueue(Camera * camera, std::vector< Renderer * > & queue);

		public:
			DefaultRenderQueue(void);
			DefaultRenderQueue(const DefaultRenderQueue &) = delete;
			virtual ~DefaultRenderQueue(void) = default;

			DefaultRenderQueue &	operator=(DefaultRenderQueue const & src) = delete;

			void		AddRenderer(Renderer * renderer) noexcept;
			uint32_t	GetQueueCount(void) noexcept;

			std::vector< Renderer * >	GetRenderersForQueue(uint32_t queueIndex) noexcept;
	};

	std::ostream &	operator<<(std::ostream & o, DefaultRenderQueue const & r);
}
