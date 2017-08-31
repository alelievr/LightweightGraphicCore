#ifndef EVENTSYSTEM_HPP
# define EVENTSYSTEM_HPP
# include <iostream>
# include <string>

class		EventSystem
{
	private:
		static onQuitCallback	_onQuit;
		static onFocusCallback	_onFocus;
		static onMoveCallback	_onMove;
		static onMouseDownCallback	_onMouseDown;
		static onMouseDragCallback	_onMouseDrag;
		static onMouseEnterCallback	_onMouseEnter;
		static onMouseExitCallback	_onMouseExit;
		static onMouseOverCallback	_onMouseOver;
		static onMouseUpCallback	_onMouseUp;


	public:
		EventSystem();
		EventSystem(const EventSystem&);
		virtual ~EventSystem(void);

		EventSystem &	operator=(EventSystem const & src);

		onQuitCallback	getOnQuit(void) const;
		void	setOnQuit(onQuitCallback tmp);
		
		onFocusCallback	getOnFocus(void) const;
		void	setOnFocus(onFocusCallback tmp);
		
		onMoveCallback	getOnMove(void) const;
		void	setOnMove(onMoveCallback tmp);
		
		onMouseDownCallback	getOnMouseDown(void) const;
		void	setOnMouseDown(onMouseDownCallback tmp);
		
		onMouseDragCallback	getOnMouseDrag(void) const;
		void	setOnMouseDrag(onMouseDragCallback tmp);
		
		onMouseEnterCallback	getOnMouseEnter(void) const;
		void	setOnMouseEnter(onMouseEnterCallback tmp);
		
		onMouseExitCallback	getOnMouseExit(void) const;
		void	setOnMouseExit(onMouseExitCallback tmp);
		
		onMouseOverCallback	getOnMouseOver(void) const;
		void	setOnMouseOver(onMouseOverCallback tmp);
		
		onMouseUpCallback	getOnMouseUp(void) const;
		void	setOnMouseUp(onMouseUpCallback tmp);
};

std::ostream &	operator<<(std::ostream & o, EventSystem const & r);

#endif
