#include "MaterialTable.hpp"

using namespace LWGC;

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

void	MaterialTable::RegsiterMaterial(Material * material)
{
	_materials.push_back(material);
}

void	MaterialTable::DestroyMaterials()
{
	printf("deivice: %p\n", VulkanInstance::Get()->GetDevice());
	for (auto material: _materials) {
		delete material;
	}
}