#include "MaterialTable.hpp"

using namespace LWGC;

std::unordered_map<ShaderProgram *, std::vector< Material * > > MaterialTable::_shadersPrograms;

MaterialTable::MaterialTable(void) : _swapChain(nullptr), _renderPass(nullptr)
{
}

MaterialTable::~MaterialTable(void)
{
}

void MaterialTable::UpdateMaterial(ShaderProgram *shaderProgram) noexcept
{
	auto materials = _shadersPrograms[shaderProgram];
	
	for (auto material: materials)
	{
		material->Update();
	}
}

void	MaterialTable::AddToList(ShaderProgram * shaderProgram, Material * material)
{
	_shadersPrograms[shaderProgram].push_back(material);
}

void 	MaterialTable::Initialize(LWGC::SwapChain *swapChain , LWGC::RenderPass *renderPipeline)
{
	_swapChain = swapChain;
	_renderPass = renderPipeline;

	for (auto material: _objects)
	{
		material->Initialize(_swapChain, _renderPass);
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