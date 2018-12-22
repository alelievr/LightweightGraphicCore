#include "Delegate.hpp"

#include <memory>

using namespace LWGC;

template<typename T>
Delegate<T>::Delegate(void)
{
}

template<typename T>
Delegate<T>::~Delegate(void)
{
}

template<typename T>
DelegateIndex<T>	Delegate<T>::AddListener(DelegateFunction<T> function) noexcept
{
	return _functionList.insert(std::make_shared< DelegateFunction<T> >(function)).first;
}

template<typename T>
void		Delegate<T>::RemoveListener(DelegateIndex<T> index) noexcept
{
	_functionList.erase(index);
}

template< typename T >
template< typename ...Params>
void		Delegate<T>::Invoke(Params && ... params) noexcept
{
	for (const auto & function : _functionList)
		(*function)(std::forward<Params>(params)...);
}

template<typename T>
std::ostream &	operator<<(std::ostream & o, Delegate<T> const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
