#ifndef MATERIAL_HPP
# define MATERIAL_HPP
# include <iostream>
# include <string>

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

		RenderQueueType	getRenderQueueType(void) const;
		void	setRenderQueueType(RenderQueueType tmp);
		
		int	getRenderFlags(void) const;
		void	setRenderFlags(int tmp);
		
		ShaderProgram	getShaderProgram(void) const;
		void	setShaderProgram(ShaderProgram tmp);
};

std::ostream &	operator<<(std::ostream & o, Material const & r);

#endif
