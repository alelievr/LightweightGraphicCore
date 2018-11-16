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

void	Mesh::AddVertex(float x, float y, float z)
{
	_vertices.push_back(glm::vec3(x, y, z));
}

void	Mesh::AddVertex(const glm::vec3 & p)
{
	_vertices.push_back(p);
}

void	Mesh::AddColor(const Color & c)
{
	_colors.push_back(c);
}

void	Mesh::AddTriangle(int p1, int p2, int p3)
{
	_triangles.push_back(p1);
	_triangles.push_back(p2);
	_triangles.push_back(p3);
}

void	Mesh::AddTangent(float x, float y, float z)
{
	_tangents.push_back(glm::vec3(x, y, z));
}

void	Mesh::AddTriangle(const glm::vec3 & t)
{
	_tangents.push_back(t);
}

void	Mesh::AddUv(float u, float v)
{
	_uvs.push_back(glm::vec2(u, v));
}

void	Mesh::AddUv(const glm::vec2 & uv)
{
	_uvs.push_back(uv);
}

void	Mesh::AddNormal(float x, float y, float z)
{
	_normals.push_back(glm::vec3(x, y, z));
}

void	Mesh::AddNormal(const glm::vec3 & n)
{
	_normals.push_back(n);
}

Bounds		Mesh::GetBounds(void) const
{
	return _bounds;
}

void		Mesh::UploadDatas(void)
{
	_instance = VulkanInstance::Get();
	_device = _instance->GetDevice();

	CreateVertexBuffer();

	// TODO: check if mesh needs index buffer
	CreateIndexBuffer();
}

void		Mesh::RecalculateBounds(void)
{
	for (const auto & v : _vertices)
	{
		_bounds.Encapsulate(v);
	}
}

void		Mesh::Clear(void)
{
	_vertices.clear();
	_normals.clear();
	_uvs.clear();
	_colors.clear();
	_tangents.clear();
}


Mesh &	Mesh::operator=(Mesh const & src)
{
	if (this != &src) {
		this->_vertices = src._vertices;
		this->_normals = src._normals;
		this->_uvs = src._uvs;
		this->_colors = src._colors;
		this->_tangents = src._tangents;
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
	VkDeviceSize bufferSize = sizeof(VertexAttributes) * vertices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	Vk::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void *data;
	vkMapMemory(_device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(_device, stagingBufferMemory);

	Vk::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _vertexBuffer, _vertexBufferMemory);
	Vk::CopyBuffer(stagingBuffer, _vertexBuffer, bufferSize);

	vkDestroyBuffer(_device, stagingBuffer, nullptr);
	vkFreeMemory(_device, stagingBufferMemory, nullptr);
}

void				Mesh::CreateIndexBuffer()
{
	VkDeviceSize bufferSize = sizeof(uint32_t) * indices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	Vk::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void *data;
	vkMapMemory(_device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indices.data(), (size_t)bufferSize);
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
	vkCmdDrawIndexed(cmd, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
}

std::vector< glm::vec3 >	Mesh::GetVertices(void) const { return (this->_vertices); }
void						Mesh::SetVertices(const std::vector< glm::vec3 > & tmp) { this->_vertices = tmp; }

std::vector< glm::vec3 >	Mesh::GetNormals(void) const { return (this->_normals); }
void						Mesh::SetNormals(const std::vector< glm::vec3 > & tmp) { this->_normals = tmp; }

std::vector< glm::vec2 >	Mesh::GetUvs(void) const { return (this->_uvs); }
void						Mesh::SetUvs(const std::vector< glm::vec2 > & tmp) { this->_uvs = tmp; }

std::vector< Color >		Mesh::GetColors(void) const { return (this->_colors); }
void						Mesh::SetColors(const std::vector< Color > & tmp) { this->_colors = tmp; }

std::vector< glm::vec3 >	Mesh::GetTangents(void) const { return (this->_tangents); }
void						Mesh::SetTangents(const std::vector< glm::vec3 > & tmp) { this->_tangents = tmp; }

std::vector< int >			Mesh::GetTriangles(void) const { return this->_triangles; }
void						Mesh::SetTriangles(const std::vector< int > & tmp) { this->_triangles = tmp; }

std::ostream &	operator<<(std::ostream & o, Mesh const & r)
{
	o << "Mesh of " << r.GetVertices().size() << " Vertices" << std::endl;
	(void)r;
	return (o);
}
