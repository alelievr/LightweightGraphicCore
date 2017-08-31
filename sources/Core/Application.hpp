#ifndef APPLICATION_HPP
# define APPLICATION_HPP
# include <iostream>
# include <string>

class		Application
{
	private:
		IRenderPipeline *	_renderPipeline;
		EventSystem	_eventSystem;


	public:
		Application();
		Application(const Application&);
		virtual ~Application(void);

		Application &	operator=(Application const & src);

		IRenderPipeline *	getRenderPipeline(void) const;
		void	setRenderPipeline(IRenderPipeline * tmp);
		
		EventSystem	getEventSystem(void) const;
		void	setEventSystem(EventSystem tmp);
};

std::ostream &	operator<<(std::ostream & o, Application const & r);

#endif
