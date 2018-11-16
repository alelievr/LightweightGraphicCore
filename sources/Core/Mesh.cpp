#include "Mesh.hpp"

#include "GLFW/glfw3.h"
#include "Core/Vulkan/Vk.hpp"

using namespace LWGC;

Mesh::Mesh(void) :	_instance(nullptr), _device(VK_NULL_HANDLE),
					_vertexBuffer(VK_NULL_HANDLE), _vertexBufferMemory(VK_NULL_HANDLE),
					_indexBuffer(VK_NULL_HANDLE), _indexBufferMemory(VK_NULL_HANDLE)
{
}

Mesh::Mesh(Mesh const & src)
{
	*this = src;
}

Mesh::~Mesh(void)
{
	if (_indexBuffer != VK_NULL_HANDLE)
	{
		vkDestroyBuffer(_device, _indexBuffer, nullptr);
		vkFreeMemory(_device, _indexBufferMemory, nullptr);
	}

	if (_vertexBuffer != VK_NULL_HANDLE)
	{
		vkDestroyBuffer(_device, _vertexBuffer, nullptr);
		vkFreeMemory(_device, _vertexBufferMemory, nullptr);
	}
}

void	Mesh::AddVertexAttribute(const VertexAttributes & attrib)
{
	_attributes.push_back(attrib);
}

void	Mesh::AddTriangle(int p1, int p2, int p3)
{
	_indices.push_back(p1);
	_indices.push_back(p2);
	_indices.push_back(p3);
}

Bounds		Mesh::GetBounds(void) const
{
	return _bounds;
}

void		Mesh::UploadDatas(void)
{
	_instance = VulkanInstance::Get();
	_device = _instance->GetDevice();

	if (_attributes.size() <= 0)
		throw std::runtime_error("Can't create a mesh with zero vertices");
	CreateVertexBuffer();

	if (_indices.size() > 0)
		CreateIndexBuffer();
}

void		Mesh::RecalculateBounds(void)
{
	for (const auto & a : _attributes)
	{
		_bounds.Encapsulate(a.position);
	}
}

void		Mesh::Clear(void)
{
	_attributes.clear();
	_indices.clear();
}


Mesh &	Mesh::operator=(Mesh const & src)
{
	if (this != &src) {
		this->_attributes = src._attributes;
		this->_indices = src._indices;
		this->_bounds = src._bounds;
	}
	return (*this);
}

// Hardcoded vertexdescriptor, TODO: API to change this
std::array< VkVertexInputAttributeDescription, 5 >	Mesh::GetAttributeDescriptions(void)
{
	std::array< VkVertexInputAttributeDescription, 5 > attributeDescriptions = {};

	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[0].offset = offsetof(VertexAttributes, position);

	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[1].offset = offsetof(VertexAttributes, normal);

	attributeDescriptions[2].binding = 0;
	attributeDescriptions[2].location = 2;
	attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[2].offset = offsetof(VertexAttributes, tangent);

	attributeDescriptions[3].binding = 0;
	attributeDescriptions[3].location = 3;
	attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[3].offset = offsetof(VertexAttributes, color);

	attributeDescriptions[4].binding = 0;
	attributeDescriptions[4].location = 4;
	attributeDescriptions[4].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[4].offset = offsetof(VertexAttributes, texCoord);

	return attributeDescriptions;
}

VkVertexInputBindingDescription						Mesh::GetBindingDescription(void)
{
	VkVertexInputBindingDescription bindingDescription = {};
	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(VertexAttributes);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return bindingDescription;
}

void				Mesh::CreateVertexBuffer()
{
	VkDeviceSize bufferSize = sizeof(VertexAttributes) * _attributes.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	Vk::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void *data;
	vkMapMemory(_device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, _attributes.data(), (size_t)bufferSize);
	vkUnmapMemory(_device, stagingBufferMemory);

	Vk::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _vertexBuffer, _vertexBufferMemory);
	Vk::CopyBuffer(stagingBuffer, _vertexBuffer, bufferSize);

	vkDestroyBuffer(_device, stagingBuffer, nullptr);
	vkFreeMemory(_device, stagingBufferMemory, nullptr);
}

void				Mesh::CreateIndexBuffer()
{
	VkDeviceSize bufferSize = sizeof(uint32_t) * _indices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	Vk::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void *data;
	vkMapMemory(_device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, _indices.data(), (size_t)bufferSize);
	vkUnmapMemory(_device, stagingBufferMemory);

	Vk::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _indexBuffer, _indexBufferMemory);
	Vk::CopyBuffer(stagingBuffer, _indexBuffer, bufferSize);

	vkDestroyBuffer(_device, stagingBuffer, nullptr);
	vkFreeMemory(_device, stagingBufferMemory, nullptr);
}

void				Mesh::BindBuffers(VkCommandBuffer cmd)
{
	VkBuffer vertexBuffers[] = {_vertexBuffer};
	VkDeviceSize offsets[] = {0};
	vkCmdBindVertexBuffers(cmd, 0, 1, vertexBuffers, offsets);
	vkCmdBindIndexBuffer(cmd, _indexBuffer, 0, VK_INDEX_TYPE_UINT32);
}

void				Mesh::Draw(VkCommandBuffer cmd)
{
	vkCmdDrawIndexed(cmd, static_cast<uint32_t>(_indices.size()), 1, 0, 0, 0);
}

std::vector< int >				Mesh::GetIndices(void) const { return _indices; }
void							Mesh::SetIndices(const std::vector< int > & tmp) { _indices = tmp; }
std::vector< Mesh::VertexAttributes >	Mesh::GetVertexAttributes(void) const { return _attributes; }
void							Mesh::SetVertexAttributes(const std::vector< Mesh::VertexAttributes > & tmp) { _attributes = tmp; }

std::ostream &	operator<<(std::ostream & o, Mesh const & r)
{
	o << "Mesh of " << r.GetVertexAttributes().size() << " Vertices" << std::endl;
	(void)r;
	return (o);
}
