#ifndef LIGHT_HPP
# define LIGHT_HPP
# include <iostream>
# include <string>

class		Light : IComponent
{
	private:


	public:
		Light();
		Light(const Light&);
		virtual ~Light(void);

		Light &	operator=(Light const & src);

		virtual void	OnAdded(const GameObject & go) noexcept;
		virtual void	OnRemoved(const GameObject & go) noexcept;
};

std::ostream &	operator<<(std::ostream & o, Light const & r);

#endif
