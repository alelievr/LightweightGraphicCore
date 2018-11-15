#include "MeshRenderer.hpp"
#include "Core/PrimitiveMeshFactory.hpp"
#include "Core/Hierarchy.hpp"

using namespace LWGC;

MeshRenderer::MeshRenderer(void)
{
}

MeshRenderer::MeshRenderer(const PrimitiveType prim)
{
	_mesh = PrimitiveMeshFactory::CreateMesh(prim);
}

MeshRenderer::~MeshRenderer(void)
{
}

Bounds		MeshRenderer::GetBounds(void)
{
	// TODO
	return Bounds();
}

void		MeshRenderer::SetModel(const Mesh & mesh, const Material & material)
{
	this->_mesh = std::make_shared< Mesh >(mesh);
	this->_material = std::make_shared< Material >(material);
}

void		MeshRenderer::SetModel(std::shared_ptr< Mesh > mesh, std::shared_ptr< Material > material)
{
	this->_mesh = mesh;
	this->_material = material;
}

void MeshRenderer::OnEnable() noexcept
{
	renderContextIndex = hierarchy->RegisterComponentInRenderContext(RenderComponentType::MeshRenderer, this);
}

void MeshRenderer::OnDisable() noexcept
{
	hierarchy->UnregisterComponentInRenderContext(RenderComponentType::MeshRenderer, renderContextIndex);
}

std::shared_ptr< Mesh >		MeshRenderer::GetMesh(void) const { return (this->_mesh); }
void		MeshRenderer::SetMesh(std::shared_ptr< Mesh > tmp) { this->_mesh = tmp; }

std::shared_ptr< Material >		MeshRenderer::GetMaterial(void) const { return (this->_material); }
void		MeshRenderer::SetMaterial(std::shared_ptr< Material > tmp) { this->_material = tmp; }

std::ostream &	operator<<(std::ostream & o, MeshRenderer const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
