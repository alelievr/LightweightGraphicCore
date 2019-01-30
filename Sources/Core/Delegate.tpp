#pragma once

#include <iostream>
#include <string>
#include <unordered_set>
#include <functional>
#include <memory>

namespace LWGC
{
	// C++ ¯\_(ツ)_/¯
	template< typename T >
	using DelegateFunction = std::function<T>;
	template< typename T >	
	using DelegateSet = std::unordered_set< std::shared_ptr< DelegateFunction< T > > >;
	template< typename T >	
	using DelegateIndex = typename DelegateSet< T >::iterator;

	template< typename T = void(void) >
	class		Delegate
	{
		private:
			DelegateSet<T>		_functionList;

		public:
			Delegate(void) {}
			Delegate(const Delegate &) = delete;
			virtual ~Delegate(void) {}

			Delegate &	operator=(Delegate const & src) = delete;

			DelegateIndex<T>	AddListener(DelegateFunction<T> function) noexcept
			{
				return _functionList.insert(std::make_shared< DelegateFunction<T> >(function)).first;				
			}
			
			void			RemoveListener(DelegateIndex<T> index) noexcept
			{
				_functionList.erase(index);				
			}

			template< typename ...Params>
			void	Invoke(Params && ... params) noexcept
			{
				for (const auto & function : _functionList)
					(*function)(std::forward<Params>(params)...);
			}
	};

	template< typename T >
	std::ostream &	operator<<(std::ostream & o, Delegate<T> const & r)
	{
		o << "tostring of the class" << std::endl;
		(void)r;
		return (o);
	}
}
