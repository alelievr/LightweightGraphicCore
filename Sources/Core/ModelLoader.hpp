#pragma once

#include <iostream>
#include <string>

#include "IncludeDeps.hpp"
#include "Core/Mesh.hpp"
#include "Core/Textures/Texture.hpp"
#include "Core/Vulkan/Material.hpp"

#include "Core/GameObject.hpp"

namespace LWGC
{
	class		ModelLoader
	{
		private:

		public:
			ModelLoader(void) = delete;
			ModelLoader(const ModelLoader&) = delete;
			~ModelLoader(void) = delete;

			ModelLoader &	operator=(ModelLoader const & src) = delete;

			static GameObject *	Load(const std::string & path, bool optimize = false) noexcept;
	};
}