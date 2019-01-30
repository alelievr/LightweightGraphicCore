#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <array>

#include "IncludeDeps.hpp"

#define GLFW_INCLUDE_GLCOREARB
#include GLFW_INCLUDE
#include GLM_INCLUDE_QUATERNION
#include GLM_INCLUDE
#include VULKAN_INCLUDE
#include "../Utils/Color.hpp"
#include "../Utils/Bounds.hpp"
#include "PrimitiveType.hpp"
#include "Core/Vulkan/VulkanInstance.hpp"

namespace LWGC
{
	class		Mesh
	{

		public:
			struct VertexAttributes
			{
				glm::vec3 position;
				glm::vec3 normal;
				glm::vec3 tangent;
				glm::vec3 color;
				glm::vec2 texCoord;

				static void QuadVertexAttrib(const glm::vec3 & p0, const glm::vec3 & p1, const glm::vec3 & p2, const glm::vec3 & p3, Mesh::VertexAttributes * targetAttribs) noexcept;
				static void QuadVertexAttrib(float size, const glm::vec3 & normal, Mesh::VertexAttributes * targetAttribs) noexcept;
				static void TriVertexAttrib(const glm::vec3 & p0, const glm::vec3 & p1, const glm::vec3 & p2, Mesh::VertexAttributes * targetAttribs) noexcept;
				static void EdgeVertexAttrib(const glm::vec3 & p0, const glm::vec3 & p1, Mesh::VertexAttributes * targetAttribs) noexcept;
			};

			Mesh(void);
			Mesh(const Mesh &);
			virtual ~Mesh(void);

			Mesh &	operator=(Mesh const & src);

			Bounds	GetBounds(void) const;

			void	AddVertexAttribute(const VertexAttributes & attrib);
			void	AddTriangle(int p1, int p2, int p3);

			void	RecalculateBounds(void);
			void	UploadDatas(void);
			void	BindBuffers(VkCommandBuffer cmd);
			void	Draw(VkCommandBuffer cmd);
			void	Clear(void);

			// transform operation on vertices
			void	Translate(const glm::vec3 & translation);
			void	Rotate(const glm::quat & rotation);

			std::vector< int >			GetIndices(void) const;
			void						SetIndices(const std::vector< int > & tmp);
			std::vector< VertexAttributes >	GetVertexAttributes(void) const;
			void						SetVertexAttributes(const std::vector< VertexAttributes > & tmp);

			static std::array< VkVertexInputAttributeDescription, 5 >	GetAttributeDescriptions(void);
			static std::array< VkVertexInputBindingDescription, 1 >		GetBindingDescription(void);

		private:
			std::vector< int >			_indices;
			std::vector< VertexAttributes >	_attributes;
			Bounds						_bounds;
			VulkanInstance *			_instance;
			VkDevice					_device;

			VkBuffer					_vertexBuffer;
			VkDeviceMemory				_vertexBufferMemory;
			VkBuffer					_indexBuffer;
			VkDeviceMemory				_indexBufferMemory;

			void		CreateVertexBuffer();
			void		CreateIndexBuffer();

	};

	std::ostream &	operator<<(std::ostream & o, Mesh const & r);
}
