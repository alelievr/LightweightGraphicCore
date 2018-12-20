#include "MaterialTable.hpp"

MaterialTable::MaterialTable(void)
{
	std::cout << "Default constructor of MaterialTable called" << std::endl;
}

MaterialTable::~MaterialTable(void)
{
	std::cout << "Destructor of MaterialTable called" << std::endl;
}

std::ostream &	operator<<(std::ostream & o, MaterialTable const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}

void	MaterialTable::RegsiterMaterial(std::shared_ptr< LWGC::Material > material)
{
	_materials.push_back(material);
}

void	MaterialTable::DestroyMaterials()
{
	for(auto& material: _materials) {
		material.reset();
	}
}