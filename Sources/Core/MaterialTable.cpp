#include "MaterialTable.hpp"

using namespace LWGC;

std::unordered_map<ShaderProgram *, std::vector< Material * > > MaterialTable::_shadersPrograms;
MaterialTable * _instance = nullptr;

MaterialTable *	MaterialTable::Get(void) { return _instance; }

MaterialTable::MaterialTable(void) : _swapChain(nullptr), _renderPass(nullptr), _initialized(false)
{
	_instance = this;
}

MaterialTable::~MaterialTable(void)
{
}

void MaterialTable::UpdateMaterial(ShaderProgram *shaderProgram) noexcept
{
	auto materials = _shadersPrograms[shaderProgram];

	for (auto material: materials)
	{
		try {
			material->ReloadShaders();
		} catch (const std::runtime_error & e) {
			std::cout << e.what() << std::endl;
		}
	}
}

void	MaterialTable::RegsiterObject(Material * material)
{
	ObjectTable::RegsiterObject(material);
	_shadersPrograms[material->GetShaderProgram()].push_back(material);
}

void 	MaterialTable::Initialize(LWGC::SwapChain *swapChain , LWGC::RenderPass *renderPipeline)
{
	_swapChain = swapChain;
	_renderPass = renderPipeline;

	for (auto material : _objects)
	{
		material->Initialize(_swapChain, _renderPass);
	}

	_initialized = true;
}

void	MaterialTable::NotifyMaterialReady(Material * material)
{
	if (_renderPass && _swapChain)
	{
		material->Initialize(_swapChain, _renderPass);
	}
}

void	MaterialTable::RecreateAll(void)
{
	for (auto material : _objects)
	{
		material->CleanupPipelineAndLayout();
		material->CreatePipelineLayout();
		material->CreatePipeline();
	}
}

void	MaterialTable::SetRenderPass(RenderPass * renderPass) { _renderPass = renderPass; }
bool	MaterialTable::IsInitialized(void) const noexcept { return _initialized; }

std::ostream &	operator<<(std::ostream & o, MaterialTable const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}