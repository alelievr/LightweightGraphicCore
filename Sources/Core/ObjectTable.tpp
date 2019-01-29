#pragma once

#include <iostream>
#include <string>
#include <unordered_set>

template< typename T = void(void) >
class		ObjectTable
{
	private:
	protected:
		std::unordered_set< T * >	_objects;

	public:
		ObjectTable(void) {}
		ObjectTable(const ObjectTable&) = delete;
		virtual ~ObjectTable(void) { }

		virtual void	RegsiterObject(T * object)
		{
			_objects.insert(object);
		}
		
		void			UnregisterObject(T * object)
		{
			_objects.erase(object);
		}

		void			DestroyObjects(void)
		{
			for (auto object: _objects) {
				delete object;
			}
		}

		ObjectTable &	operator=(ObjectTable const & src) = delete;
};

template< typename T >
std::ostream &	operator<<(std::ostream & o, ObjectTable<T> const & r)
{
		o << "tostring of the class ObjectTable" << std::endl;
		(void)r;
		return (o);
}