namespace LWGE
{
	namespace Utils
	{
		class Bounds
		{
			private:
				glm::vec3	min;
				glm::vec3	max;

			public:
				Bounds(void);
				Bounds(const glm::vec3 & min, const glm::vec3 & max);
				Bounds(const float x1, const float y1, const float z1, const float x2, const float y2, const float z2);
				Bounds(const Bounds & rhs);
				
				Bounds & operator=(const Bounds & rhs);

				float		GetMaxX(void) const;
				float		GetMaxY(void) const;
				float		GetMaxZ(void) const;

				float		GetMinX(void) const;
				float		GetMinY(void) const;
				float		GetMinZ(void) const;

				glm::vec3	GetSize(void) const;

				//...
		}
	}
}
