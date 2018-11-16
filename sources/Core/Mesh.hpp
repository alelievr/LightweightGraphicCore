#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <array>

#include "IncludeDeps.hpp"

#define GLFW_INCLUDE_GLCOREARB
#include GLFW_INCLUDE

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

			std::vector< int >			GetIndices(void) const;
			void						SetIndices(const std::vector< int > & tmp);
			std::vector< VertexAttributes >	GetVertexAttributes(void) const;
			void						SetVertexAttributes(const std::vector< VertexAttributes > & tmp);
			
			static std::array< VkVertexInputAttributeDescription, 5 >	GetAttributeDescriptions(void);
			static VkVertexInputBindingDescription						GetBindingDescription(void);
			
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
