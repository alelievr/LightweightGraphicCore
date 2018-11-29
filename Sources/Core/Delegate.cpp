#include "Delegate.hpp"

#include <memory>

using namespace LWGC;

Delegate::Delegate(void)
{
}

Delegate::~Delegate(void)
{
}

DelegateIndex	Delegate::AddListener(DelegateFunction function) noexcept
{
	return _functionList.insert(std::make_shared< DelegateFunction >(function)).first;
}

void		Delegate::RemoveListener(DelegateIndex index) noexcept
{
	_functionList.erase(index);
}

void		Delegate::Invoke(void) noexcept
{
	for (const auto & function : _functionList)
		(*function)();
}

std::ostream &	operator<<(std::ostream & o, Delegate const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
