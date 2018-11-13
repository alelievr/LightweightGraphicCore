#include "MeshRenderer.hpp"
#include "Core/PrimitiveMeshFactory.hpp"
#include <memory>

using namespace LWGC;

MeshRenderer::MeshRenderer(void)
{
	std::cout << "Default constructor of MeshRenderer called" << std::endl;
}

MeshRenderer::MeshRenderer(MeshRenderer const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

MeshRenderer::MeshRenderer(const PrimitiveType prim)
{
	_mesh = PrimitiveMeshFactory::CreateMesh(prim);
}

MeshRenderer::~MeshRenderer(void)
{
	std::cout << "Destructor of MeshRenderer called" << std::endl;
}

SortingLayer		MeshRenderer::GetSortingLayer(void)
{
}

Bounds		MeshRenderer::GetBounds(void)
{
	
}

void		MeshRenderer::Render(void)
{
	std::cout << "Render model !\n";
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

MeshRenderer &	MeshRenderer::operator=(MeshRenderer const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
		this->_mesh = src.GetMesh();
		this->_material = src.GetMaterial();
	}
	return (*this);
}

void MeshRenderer::OnRemoved(const GameObject & go) noexcept
{
	(void)go;
	// TODO: register this component in the renderable list of the application
}

void MeshRenderer::OnAdded(const GameObject & go) noexcept
{
	(void)go;
	// TODO: remove this component from the renderable list
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
