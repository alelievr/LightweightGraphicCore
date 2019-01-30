#include "MeshRenderer.hpp"
#include "Core/PrimitiveMeshFactory.hpp"
#include "Core/Hierarchy.hpp"
#include "Core/Rendering/VulkanRenderPipeline.hpp"
#include "Core/Vulkan/VulkanInstance.hpp"

using namespace LWGC;

MeshRenderer::MeshRenderer(const PrimitiveType prim, Material * material) : Renderer(material)
{
	_mesh = PrimitiveMeshFactory::CreateMesh(prim);
}

MeshRenderer::MeshRenderer(Material * material) : Renderer(material)
{
}

MeshRenderer::MeshRenderer(const PrimitiveType prim) : Renderer(Material::Create(BuiltinShaders::Pink))
{
	_mesh = PrimitiveMeshFactory::CreateMesh(prim);
}

MeshRenderer::~MeshRenderer(void)
{
	_mesh.reset();
}

void		MeshRenderer::Initialize(void) noexcept
{
	_mesh->UploadDatas();
	Renderer::Initialize();
}

void		MeshRenderer::RecordDrawCommand(VkCommandBuffer cmd) noexcept
{
	_mesh->BindBuffers(cmd);

	_mesh->Draw(cmd);
}

void		MeshRenderer::SetModel(const Mesh & mesh, Material * material)
{
	this->_mesh = std::make_shared< Mesh >(mesh);
	this->_material = material;
}

void		MeshRenderer::SetModel(std::shared_ptr< Mesh > mesh, Material * material)
{
	this->_mesh = mesh;
	this->_material = material;
}

Bounds		MeshRenderer::GetBounds(void) noexcept
{
	return _mesh->GetBounds();
}

uint32_t					MeshRenderer::GetType(void) const noexcept { return static_cast< uint32_t >(ComponentType::MeshRenderer); }

std::shared_ptr< Mesh >		MeshRenderer::GetMesh(void) const { return (this->_mesh); }
void						MeshRenderer::SetMesh(std::shared_ptr< Mesh > tmp) { this->_mesh = tmp; }

std::ostream &	operator<<(std::ostream & o, MeshRenderer const & r)
{
	o << "MeshRenderer" << std::endl;
	(void)r;
	return (o);
}
