#include "GizmoBase.hpp"

using namespace LWGC::Gizmo;

static const std::string GizmoShader = "Shaders/Gizmo/Default.hlsl";

GizmoBase::GizmoBase(void)
{
	material = Material::Create(GizmoShader);
	renderer = new MeshRenderer(material);
	AddComponent(renderer);
}

GizmoBase::~GizmoBase(void)
{
}
