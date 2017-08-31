#ifndef OBJECT_HPP
# define OBJECT_HPP
# include <iostream>
# include <string>

class		Object
{
	private:
		Trasform	_trasform;
		std::string	_name;
		int	_flags;

	public:
		Object(void);
		Object(const Object&);
		virtual ~Object(void);

		Object &	operator=(Object const & src);

		Trasform	getTrasform(void) const;
		void	setTrasform(Trasform tmp);
		
		std::string	getName(void) const;
		void		setName(std::string tmp);
		
		int		getFlags(void) const;
		void	setFlags(int tmp);
};

std::ostream &	operator<<(std::ostream & o, Object const & r);

#endif
