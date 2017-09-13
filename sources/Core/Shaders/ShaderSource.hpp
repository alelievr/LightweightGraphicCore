#ifndef SHADERSOURCE_HPP
# define SHADERSOURCE_HPP
# include <iostream>
# include <string>

class		ShaderSource
{
	private:
		std::list< ShaderFileInfo >	_sourceFiles;


	public:
		ShaderSource();
		ShaderSource(const ShaderSource&);
		virtual ~ShaderSource(void);

		ShaderSource &	operator=(ShaderSource const & src);

		void	AddSourceFile(const std::string source);

		void	AddSource(const std::string source);

		bool	NeedReload(void);

		const GLchar **	GetVertexSources(int *nSources);

		const GLchar **	GetGeometrySources(int *nSources);

		const GLchar **	GetFragmentSources(int *nSources);

		std::list< ShaderFileInfo >	GetSourceFiles(void) const;
		void	SetSourceFiles(std::list< ShaderFileInfo > tmp);
};

std::ostream &	operator<<(std::ostream & o, ShaderSource const & r);

#endif
