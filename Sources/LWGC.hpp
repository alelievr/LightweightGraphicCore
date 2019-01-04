/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LWGC.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amerelo <amerelo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/27 21:37:58 by alelievr          #+#    #+#             */
/*   Updated: 2018/12/21 17:42:44 by amerelo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

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
#include "Core/Events/EventSystem.hpp"
#include "Core/Events/Event.hpp"
#include "Core/Events/Input.hpp"
#include "Core/KeyCode.hpp"

// Components
#include "Core/Components/Component.hpp"
#include "Core/Components/MeshRenderer.hpp"
#include "Core/Components/ProceduralRenderer.hpp"
#include "Core/Components/Camera.hpp"
#include "Core/Components/FreeCameraControls.hpp"
#include "Core/Components/ComputeDispatcher.hpp"
#include "Core/Components/Rotator.hpp"

// Gimos & handles
#include "Core/Gizmos/Line.hpp"
#include "Core/Gizmos/Ray.hpp"
#include "Core/Gizmos/Frustum.hpp"

// Utils
#include "Utils/Bounds.hpp"
#include "Utils/Rect.hpp"