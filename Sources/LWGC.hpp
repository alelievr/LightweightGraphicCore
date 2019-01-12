/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LWGC.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amerelo <amerelo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/27 21:37:58 by alelievr          #+#    #+#             */
/*   Updated: 2019/01/05 18:42:25 by amerelo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#define LWGC_GLOBAL_INCLUDE

#include "IncludeDeps.hpp"

// Pipelines
#include "Core/Rendering/VulkanRenderPipeline.hpp"

// Rendering
#include "Core/Rendering/RenderTarget.hpp"
#include "Core/Mesh.hpp"
#include "Core/PrimitiveType.hpp"
#include "Core/PrimitiveMeshFactory.hpp"
#include "Core/Textures/Texture2D.hpp"
#include "Core/Textures/Texture3D.hpp"
#include "Core/Vulkan/Material.hpp"
#include "Core/Shaders/BuiltinShaders.hpp"

// App & core
#include "Core/Application.hpp"
#include "Core/Hierarchy.hpp"
#include "Core/Transform.hpp"
#include "Core/GameObject.hpp"

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
#include "Core/Components/Rotator.hpp"
#include "Core/Components/Movator.hpp"

// Gimos & handles
#include "Core/Gizmos/Line.hpp"
#include "Core/Gizmos/Ray.hpp"
#include "Core/Gizmos/Frustum.hpp"
#include "Core/Gizmos/Circle.hpp"
#include "Core/Gizmos/Cone.hpp"
#include "Core/Gizmos/Arrow.hpp"
#include "Core/Gizmos/Position.hpp"

// Utils
#include "Utils/Bounds.hpp"
#include "Utils/Rect.hpp"
#include "Utils/Math.hpp"