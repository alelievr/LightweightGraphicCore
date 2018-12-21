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
	using DelegateSet = std::unordered_set< std::shared_ptr< DelegateFunction > >;
	using DelegateIndex = DelegateSet::iterator;

	using Delegate = Delegate< void(void) >;

	template< typename T >
	class		Delegate< T >
	{
		private:
			DelegateSet		_functionList;

		public:
			Delegate(void);
			Delegate(const Delegate &) = delete;
			virtual ~Delegate(void);

			Delegate &	operator=(Delegate const & src) = delete;

			DelegateIndex	AddListener(DelegateFunction function) noexcept;
			void			RemoveListener(DelegateIndex index) noexcept;

			void	Invoke(void) noexcept;
	};

	std::ostream &	operator<<(std::ostream & o, Delegate const & r);
}
