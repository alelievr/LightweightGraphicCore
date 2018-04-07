#pragma once

#include <iostream>
#include <string>

#include "Shaders/ShaderProgram.hpp"
#include "Texture.hpp"
#include "Rendering/RenderQueueType.hpp"
#include "../Utils/Color.hpp"
#include <glm/glm.hpp>
#include "Rendering/RenderMode.hpp"

namespace LWGE
{
	class		Material
	{
		private:
			RenderQueueType	_renderQueueType;
			RenderMode		_renderMode;
			ShaderProgram	_shaderProgram;


		public:
			Material(void);
			Material(const Material&);
			virtual ~Material(void);

			Material &	operator=(Material const & src);

			void	Bind(void);

			void	SetColor(const std::string & name, const Color & c);

			void	SetFloat(const std::string & name, const float f);

			void	SetInt(const std::string & name, const int i);

			void	SetTexture(const std::string & name, const Texture & t);

			void	SetMatrix(const std::string & name, const glm::mat4 & m);

			RenderQueueType	GetRenderQueueType(void) const;
			void	SetRenderQueueType(RenderQueueType tmp);
			
			RenderMode	GetRenderMode(void) const;
			void	SetRenderMode(RenderMode tmp);
			
			ShaderProgram	GetShaderProgram(void) const;
			void	SetShaderProgram(ShaderProgram tmp);
	};

	std::ostream &	operator<<(std::ostream & o, Material const & r);
}
