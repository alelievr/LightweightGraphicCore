#pragma once

#include <iostream>
#include <string>

#include "WindowFlag.hpp"
#include "Rendering/IRenderPipeline.hpp"
#include "Events/EventSystem.hpp"

#define GLFW_INCLUDE_GLCOREARB
#include GLFW_INCLUDE

namespace LWGC
{
	class		Application
	{
		private:
			std::shared_ptr< IRenderPipeline >	_renderPipeline;
			std::unique_ptr< EventSystem >		_eventSystem;
			GLFWwindow *						_window;

			bool	_shouldNotQuit;

		public:
			Application(void);
			Application(const Application&) = delete;
			virtual ~Application(void);

			Application &	operator=(Application const & src) = delete;

			void	Init(void) noexcept;
			bool	ShouldNotQuit(void) const noexcept;
			void	Quit(void) noexcept;
			void	Open(const std::string & name, const int width, const int height, const WindowFlag flags) noexcept;
			void	Update(void) noexcept;

			IRenderPipeline &	GetRenderPipeline(void) const noexcept;
			void	SetRenderPipeline(std::shared_ptr< IRenderPipeline > tmp) noexcept;
			
			EventSystem &		GetEventSystem(void) const noexcept;
	};

	std::ostream &	operator<<(std::ostream & o, Application const & r);
}
