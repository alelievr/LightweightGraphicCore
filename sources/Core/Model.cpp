#include "Model.hpp"
#include "PrimitiveMeshFactory.hpp"
#include <memory>

using namespace LWGC;

Model::Model(void)
{
	std::cout << "Default constructor of Model called" << std::endl;
}

Model::Model(Model const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

Model::Model(const PrimitiveType prim)
{
	_mesh = PrimitiveMeshFactory::CreateMesh(prim);
}

Model::~Model(void)
{
	std::cout << "Destructor of Model called" << std::endl;
}

SortingLayer		Model::GetSortingLayer(void)
{
	
}

Bounds		Model::GetBounds(void)
{
	
}

void		Model::Render(void)
{
	std::cout << "Render model !\n";
}

void		Model::SetModel(const Mesh & mesh, const Material & material)
{
	this->_mesh = std::make_shared< Mesh >(mesh);
	this->_material = std::make_shared< Material >(material);
}

void		Model::SetModel(std::shared_ptr< Mesh > mesh, std::shared_ptr< Material > material)
{
	this->_mesh = mesh;
	this->_material = material;
}

Model &	Model::operator=(Model const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
		this->_mesh = src.GetMesh();
		this->_material = src.GetMaterial();
	}
	return (*this);
}

std::shared_ptr< Mesh >		Model::GetMesh(void) const { return (this->_mesh); }
void		Model::SetMesh(std::shared_ptr< Mesh > tmp) { this->_mesh = tmp; }

std::shared_ptr< Material >		Model::GetMaterial(void) const { return (this->_material); }
void		Model::SetMaterial(std::shared_ptr< Material > tmp) { this->_material = tmp; }

std::ostream &	operator<<(std::ostream & o, Model const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
