#pragma once

#include <iostream>
#include <string>

#include "Transform.hpp"

namespace LWGC
{
	class		Object
	{
		private:
			Transform	_trasform;
			std::string	_name;
			int			_flags;

		public:
			Object(void);
			Object(const Object&);
			virtual		~Object(void);

			Object &	operator=(Object const & src);

			Transform	GetTransform(void) const;
			void		SetTransform(Transform tmp);
			
			std::string	GetName(void) const;
			void		SetName(std::string newName);
			
			int			GetFlags(void) const;
			void		SetFlags(int tmp);
	};

	std::ostream &	operator<<(std::ostream & o, Object const & r);
}
