namespace LWGC
{
	enum class WindowFlag : int
	{
		Resizable =	0x001,
		Decorated =	0x002,
		Focused =	0x004,
		Floating =	0x008,
		Maximized =	0x010,
	};

	inline WindowFlag operator | (const WindowFlag lhs, const WindowFlag rhs)
	{
		return static_cast< WindowFlag >(static_cast< int >(lhs) | static_cast< int >(rhs));
	}

	inline WindowFlag operator & (const WindowFlag lhs, const WindowFlag rhs)
	{
		return static_cast< const WindowFlag >(static_cast< int >(lhs) & static_cast< int >(rhs));
	}

	inline int operator !=(WindowFlag lhs, int val)
	{
		return static_cast< int >(lhs) != val;
	}

	inline int operator ==(WindowFlag lhs, int val)
	{
		return static_cast< int >(lhs) == val;
	}

}
