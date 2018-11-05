#pragma once
#include <iostream>
#include <string>

#include "TextureFilterMode.hpp"
#include "TextureWrapMode.hpp"

namespace LWGC
{
	class		Texture
	{
		private:
			std::size_t			_width;
			std::size_t			_height;
			TextureFilterMode	_filterMode;
			TextureWrapMode		_wrapMode;


		public:
			Texture(void);
			Texture(const Texture&);
			virtual ~Texture(void);

			Texture &	operator=(Texture const & src);

			std::size_t	GetWidth(void) const;
			void	SetWidth(std::size_t tmp);
			
			std::size_t	GetHeight(void) const;
			void	SetHeight(std::size_t tmp);
			
			TextureFilterMode	GetFilterMode(void) const;
			void	SetFilterMode(TextureFilterMode tmp);
			
			TextureWrapMode	GetWrapMode(void) const;
			void	SetWrapMode(TextureWrapMode tmp);
	};

	std::ostream &	operator<<(std::ostream & o, Texture const & r);
}
