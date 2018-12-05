/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LWGC.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/27 21:37:58 by alelievr          #+#    #+#             */
/*   Updated: 2018/11/13 22:54:06 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define LWGC_GLOBAL_INCLUDE

#include "IncludeDeps.hpp"

// Pipelines
#include "Core/Rendering/VulkanRenderPipeline.hpp"

// Shaders & materials
#include "Core/Shaders/ShaderProgram.hpp"
#include "Core/Shaders/ShaderSource.hpp"
#include "Core/Vulkan/Material.hpp"

// Rendering
#include "Core/Rendering/RenderTarget.hpp"
#include "Core/Mesh.hpp"
#include "Core/PrimitiveType.hpp"
#include "Core/PrimitiveMeshFactory.hpp"

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
#include "Core/Components/Camera.hpp"
#include "Core/Components/FreeCameraControls.hpp"
#include "Core/Components/ComputeDispatcher.hpp"

// Utils
#include "Utils/Bounds.hpp"
#include "Utils/Rect.hpp"
