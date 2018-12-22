#pragma once

#include <iostream>
#include <string>
#include <unordered_set>
#include <functional>

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
			Delegate(void);
			Delegate(const Delegate &) = delete;
			virtual ~Delegate(void);

			Delegate &	operator=(Delegate const & src) = delete;

			DelegateIndex<T>	AddListener(DelegateFunction<T> function) noexcept;
			void			RemoveListener(DelegateIndex<T> index) noexcept;

			template< typename ...Params>
			void	Invoke(Params && ... params) noexcept;
	};

	template< typename T >
	std::ostream &	operator<<(std::ostream & o, Delegate<T> const & r);
}
