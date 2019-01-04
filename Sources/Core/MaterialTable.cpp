#include "MaterialTable.hpp"

using namespace LWGC;

MaterialTable::MaterialTable(void)
{
}

MaterialTable::~MaterialTable(void)
{
}

std::ostream &	operator<<(std::ostream & o, MaterialTable const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}

void	MaterialTable::RegsiterMaterial(Material * material)
{
	_materials.push_back(material);
}

void	MaterialTable::DestroyMaterials()
{
	for (auto material: _materials) {
		delete material;
	}
}