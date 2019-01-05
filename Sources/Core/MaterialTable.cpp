#include "MaterialTable.hpp"

using namespace LWGC;

MaterialTable::MaterialTable(void) : _swapChain(nullptr), _renderPass(nullptr)
{
}

MaterialTable::~MaterialTable(void)
{
}

void 	MaterialTable::Initialize(LWGC::SwapChain *swapChain , LWGC::RenderPass *renderPipeline)
{
	_swapChain = swapChain;
	_renderPass = renderPipeline;

	for (auto material: _materials)
	{
		material->Initialize(_swapChain, _renderPass);
	}
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

void	MaterialTable::NotifyMaterialReady(Material * material)
{
	if (_renderPass && _swapChain)
	{
		material->Initialize(_swapChain, _renderPass);
	}
}

std::ostream &	operator<<(std::ostream & o, MaterialTable const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}