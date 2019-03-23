#pragma once

#include <iostream>
#include <string>

#include "IncludeDeps.hpp"

#include "Core/GameObject.hpp"
#include ASSIMP_SCENE_INCLUDE

namespace LWGC
{
	class		ModelLoader
	{
		private:
			static GameObject *	SceneToGameObject(const aiScene * scene);

		public:
			ModelLoader(void) = delete;
			ModelLoader(const ModelLoader&) = delete;
			~ModelLoader(void) = delete;

			ModelLoader &	operator=(ModelLoader const & src) = delete;

			static GameObject *	Load(const std::string & path, bool optimize = false) noexcept;
	};
}