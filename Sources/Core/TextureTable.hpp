#pragma once

#include <iostream>
#include <string>
#include "Core/ObjectTable.tpp"
#include "Core/Textures/Texture.hpp"

namespace LWGC
{
	class		TextureTable : public ObjectTable<Texture>
	{
		private:

		public:
			TextureTable(void);
			TextureTable(const TextureTable&) = delete;
			virtual ~TextureTable(void);

			TextureTable &	operator=(TextureTable const & src) = delete;
	};

	std::ostream &	operator<<(std::ostream & o, TextureTable const & r);
}