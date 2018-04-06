#pragma once

#include <iostream>
#include <string>

#include "WindowFlag.hpp"
#include "IRenderPipeline.hpp"
#include "EventSystem.hpp"

#define GLFW_INCLUDE_GLCOREARB
#include "GLFW/glfw3.h"

namespace LWGE
{
	class		Application
	{
		private:
			IRenderPipeline *	_renderPipeline;
			EventSystem			_eventSystem;
			bool				_shouldNotQuit;
			GLFWwindow *		_window;
			

		public:
			Application(void);
			Application(const Application&) = delete;
			virtual ~Application(void);

			Application &	operator=(Application const & src) = delete;

			bool	ShouldNotQuit() const noexcept;
			void	Quit() noexcept;
			void	Open(const std::string & name, const int width, const int height, const WindowFlag flags) noexcept;

			IRenderPipeline *	GetRenderPipeline(void) const noexcept;
			void				SetRenderPipeline(IRenderPipeline * tmp) noexcept;
			
			const EventSystem *	GetEventSystem(void) const noexcept;
			void				SetEventSystem(EventSystem tmp) noexcept;
	};

	std::ostream &	operator<<(std::ostream & o, Application const & r);
}
