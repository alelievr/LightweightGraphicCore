#pragma once

#include <iostream>
#include <string>

#include "Core/Gizmos/GizmoBase.hpp"

namespace LWGC::Gizmo
{
	class		Line : public GizmoBase
	{
		private:


		public:
			Line();
			Line(const Line&);
			virtual ~Line(void);

			Line &	operator=(Line const & src);
	};

	std::ostream &	operator<<(std::ostream & o, Line const & r);
}
