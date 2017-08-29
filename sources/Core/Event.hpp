#ifndef EVENT_HPP
# define EVENT_HPP
# include <iostream>
# include <string>

class		Event
{
	private:
		EventType	_type;
		bool	_alt;
		bool	_control;
		bool	_shift;
		bool	_function;
		bool	_capsLock;
		int	_keyCode;
		int	_mouseButton;
		int	_joystickNumber;
		glm::vec2	_delta;


	public:
		Event();
		Event(const Event&);
		virtual ~Event(void);

		Event &	operator=(Event const & src);

		EventType	getType(void) const;
		void	setType(EventType tmp);
		
		bool	getAlt(void) const;
		void	setAlt(bool tmp);
		
		bool	getControl(void) const;
		void	setControl(bool tmp);
		
		bool	getShift(void) const;
		void	setShift(bool tmp);
		
		bool	getFunction(void) const;
		void	setFunction(bool tmp);
		
		bool	getCapsLock(void) const;
		void	setCapsLock(bool tmp);
		
		int	getKeyCode(void) const;
		void	setKeyCode(int tmp);
		
		int	getMouseButton(void) const;
		void	setMouseButton(int tmp);
		
		int	getJoystickNumber(void) const;
		void	setJoystickNumber(int tmp);
		
		glm::vec2	getDelta(void) const;
		void	setDelta(glm::vec2 tmp);
};

std::ostream &	operator<<(std::ostream & o, Event const & r);

#endif
