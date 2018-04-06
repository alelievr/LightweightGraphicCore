#include "glm/glm.hpp"

namespace LWGE
{
	class Rect
	{
		private:
			glm::vec2	min;
			glm::vec2	max;

		public:
			Rect(void);
			Rect(const Rect & rhs);
			Rect(const glm::vec2 & min, const glm::vec2 & max);
			Rect(const float x1, const float y1, const float x2, const float y2);

			Rect & operator=(const Rect & rhs);

			float GetMaxX(void) const;
			float GetMaxY(void) const;
			float GetMinX(void) const;
			float GetMinY(void) const;

			//...
	};
}
