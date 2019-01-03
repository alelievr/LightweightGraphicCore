#pragma once

#include <iostream>
#include <string>
#include <unordered_set>
#include <functional>
#include <memory>

namespace LWGC
{
	// C++ ¯\_(ツ)_/¯
	using DelegateFunction = std::function<void()>;
	using DelegateSet = std::unordered_set< std::shared_ptr< DelegateFunction > >;
	using DelegateIndex = DelegateSet::iterator;
	
	class		Delegate
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
