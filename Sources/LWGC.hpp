/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LWGC.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amerelo <amerelo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/27 21:37:58 by alelievr          #+#    #+#             */
/*   Updated: 2019/03/03 17:21:10 by amerelo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#define LWGC_GLOBAL_INCLUDE

#include "IncludeDeps.hpp"

// Pipelines
#include "Core/Rendering/RenderPipeline.hpp"

// Rendering
#include "Core/Rendering/RenderTarget.hpp"
#include "Core/Mesh.hpp"
#include "Core/PrimitiveType.hpp"
#include "Core/PrimitiveMeshFactory.hpp"
#include "Core/Textures/Texture2D.hpp"
#include "Core/Textures/Texture3D.hpp"
#include "Core/Vulkan/Material.hpp"
#include "Core/Shaders/BuiltinShaders.hpp"
#include "Core/Vulkan/MaterialStates.hpp"

// App & core
#include "Core/Application.hpp"
#include "Core/Hierarchy.hpp"
#include "Core/Transform.hpp"
#include "Core/GameObject.hpp"
#include "Core/Time.hpp"
#include "Core/Texture2DAtlas.hpp"
#include "Core/NodeTree.hpp"
#include "Core/ModelLoader.hpp"

// Event System
#include "Core/EventSystem.hpp"
#include "Core/KeyCode.hpp"

// Components
#include "Core/Components/Component.hpp"
#include "Core/Components/MeshRenderer.hpp"
#include "Core/Components/ProceduralRenderer.hpp"
#include "Core/Components/Camera.hpp"
#include "Core/Components/FreeCameraControls.hpp"
#include "Core/Components/ComputeDispatcher.hpp"
#include "Core/Components/ImGUIPanel.hpp"
#include "Core/Components/Rotator.hpp"
#include "Core/Components/Movator.hpp"
#include "Core/Components/Activator.hpp"
#include "Core/Components/ProfilerPanel.hpp"
#include "Core/Components/IndirectRenderer.hpp"

// Gimos & handles
#include "Core/Gizmos/Line.hpp"
#include "Core/Gizmos/Ray.hpp"
#include "Core/Gizmos/Frustum.hpp"
#include "Core/Gizmos/Circle.hpp"
#include "Core/Gizmos/Cone.hpp"
#include "Core/Gizmos/Arrow.hpp"
#include "Core/Gizmos/Position.hpp"
#include "Core/Handles/PositionHandle.hpp"

// Utils
#include "Utils/Bounds.hpp"
#include "Utils/Rect.hpp"
#include "Utils/Math.hpp"
#include "Utils/Random.hpp"

// ImGUI
#include IMGUI_INCLUDE

#undef LWGC_GLOBAL_INCLUDE