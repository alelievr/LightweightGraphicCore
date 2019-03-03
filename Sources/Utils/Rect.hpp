#pragma once
#include "IncludeDeps.hpp"

#include GLM_INCLUDE

namespace LWGC
{
	class Rect
	{
		private:
			glm::vec2	_size;
			glm::vec2	_offset;

		public:
			Rect(void);
			Rect(int w, int h, int x, int y);
			Rect(const Rect & rhs);
			Rect(const glm::vec2 & min, const glm::vec2 & max);
			Rect(const float x1, const float y1, const float x2, const float y2);

			Rect &	operator=(const Rect & rhs);
			bool	operator!=(const Rect & rhs);
			bool	operator==(const Rect & rhs);

			float 		GetMaxX(void) const;
			float 		GetMaxY(void) const;
			float 		GetMinX(void) const;
			float 		GetMinY(void) const;
			glm::vec2	GetSize();
			glm::vec2	GetOffset();

			bool	CanFit(const Rect & rect, int w, int h);
			static const Rect Invalid;
	};
}
