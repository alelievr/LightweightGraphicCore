#include "Core/Handles/HandleControl.hpp"
#include "Core/Handles/HandleManager.hpp"

#include "Utils/Utils.hpp"

using namespace LWGC;
using namespace Handle;

HandleControl::HandleControl()
{
    HandleManager::AddHandleControl(this);
}

void           HandleControl::BindGameObject(GameObject * go)
{
    go->onEnableChanged.AddListener([&](bool enabled){
        if (enabled)
            Enable();
        else
            Disable();
    });
}

void        HandleControl::Enable()
{
    HandleManager::AddHandleControl(this);
}
void        HandleControl::Disable()
{
    HandleManager::RemoveHandleControl(this);
}
