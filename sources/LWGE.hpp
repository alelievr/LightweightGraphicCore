/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LWGE.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/27 21:37:58 by alelievr          #+#    #+#             */
/*   Updated: 2018/04/07 01:03:10 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Core/Rendering/DeferredRenderPipeline.hpp"
#include "Core/Rendering/ForwardRenderPipeline.hpp"
#include "Core/Shaders/IChannel.hpp"
#include "Core/Rendering/IRenderPipeline.hpp"
#include "Core/Shaders/ImageChannel.hpp"
#include "Core/Material.hpp"
#include "Core/Mesh.hpp"
#include "Core/Model.hpp"
#include "Core/PrimitiveMeshFactory.hpp"
#include "Core/PrimitiveType.hpp"
#include "Core/Events/EventSystem.hpp"
#include "Core/Events/Event.hpp"
#include "Core/Events/Input.hpp"
#include "Core/Application.hpp"
#include "Core/Shaders/ProgramChannel.hpp"
#include "Core/Rendering/RenderQueue.hpp"
#include "Core/Rendering/RenderTarget.hpp"
#include "Core/Shaders/ShaderProgram.hpp"
#include "Core/Shaders/ShaderSource.hpp"
#include "Core/Transform.hpp"
#include "GUI/Image.hpp"
#include "GUI/Text.hpp"
#include "Utils/Bounds.hpp"
#include "Utils/Rect.hpp"
#include "Core/KeyCode.hpp"
