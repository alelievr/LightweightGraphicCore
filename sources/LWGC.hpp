/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LWGC.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/27 21:37:58 by alelievr          #+#    #+#             */
/*   Updated: 2018/11/06 17:45:57 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "IncludeDeps.hpp"

// Pipelines
#include "Core/Rendering/IRenderPipeline.hpp"
#include "Core/Rendering/ForwardRenderPipeline.hpp"

// Shaders & materials
#include "Core/Shaders/IChannel.hpp"
#include "Core/Shaders/ImageChannel.hpp"
#include "Core/Shaders/ProgramChannel.hpp"
#include "Core/Shaders/ShaderProgram.hpp"
#include "Core/Shaders/ShaderSource.hpp"
#include "Core/Material.hpp"

// Rendering
#include "Core/Rendering/RenderQueue.hpp"
#include "Core/Rendering/RenderTarget.hpp"
#include "Core/Mesh.hpp"
#include "Core/PrimitiveMeshFactory.hpp"

// App & core
#include "Core/PrimitiveType.hpp"
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
#include "Core/IComponent.hpp"
#include "Core/MeshRenderer.hpp"

// Utils
#include "Utils/Bounds.hpp"
#include "Utils/Rect.hpp"
