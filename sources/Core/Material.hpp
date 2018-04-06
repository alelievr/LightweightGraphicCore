#pragma once

#include <iostream>
#include <string>

#include "ShaderProgram.hpp"
#include "Texture.hpp"
#include "RenderQueueType.hpp"

namespace LWGE
{
	class		Material
	{
		private:
			RenderQueueType	_renderQueueType;
			int	_renderFlags;
			ShaderProgram	_shaderProgram;


		public:
			Material();
			Material(const Material&);
			virtual ~Material(void);

			Material &	operator=(Material const & src);

			void	Bind(void);

			void	SetColor(const std::string & name, const Color & c);

			void	SetFloat(const std::string & name, const float f);

			void	SetInt(const std::string & name, const int i);

			void	SetTexture(const Texture & t);

			void	SetMatrix(const std::string & name, const glm::mat4 & m);

			RenderQueueType	GetRenderQueueType(void) const;
			void	SetRenderQueueType(RenderQueueType tmp);
			
			int	GetRenderFlags(void) const;
			void	SetRenderFlags(int tmp);
			
			ShaderProgram	GetShaderProgram(void) const;
			void	SetShaderProgram(ShaderProgram tmp);
	};

	std::ostream &	operator<<(std::ostream & o, Material const & r);
}
