#include "Model.hpp"


Model::Model(void)
{
	std::cout << "Default constructor of Model called" << std::endl;
	this->_mesh = ;
	this->_materials = ;
}

Model::Model(Model const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
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
	
}


Model &	Model::operator=(Model const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
		this->_mesh = src.getMesh();
		this->_materials = src.getMaterials();
	}
	return (*this);
}

Mesh		Model::getMesh(void) const { return (this->_mesh); }
void		Model::setMesh(Mesh tmp) { this->_mesh = tmp; }

std::vector< Material >		Model::getMaterials(void) const { return (this->_materials); }
void		Model::setMaterials(std::vector< Material > tmp) { this->_materials = tmp; }

std::ostream &	operator<<(std::ostream & o, Model const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
