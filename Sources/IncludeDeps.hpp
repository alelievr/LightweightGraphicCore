#ifndef INCLUDEDEPS_HPP
# define INCLUDEDEPS_HPP

# define GLM_ENABLE_EXPERIMENTAL

# define GLM_INCLUDE "../Deps/glm/glm/glm.hpp"
# define GLM_INCLUDE_QUATERNION "../Deps/glm/glm/gtx/quaternion.hpp"
# define GLM_INCLUDE_MATRIX_TRANSFORM "../Deps/glm/glm/gtc/matrix_transform.hpp"
# define GLFW_INCLUDE "../Deps/glfw/include/GLFW/glfw3.h"
# define IMGUI_INCLUDE "../Deps/imgui/imgui.h"
# define STB_INCLUDE "../Deps/stb/stb.h"
# define STB_INCLUDE_IMAGE "../Deps/stb/stb_image.h"
// Unusable API
// # define GLSLANG_INCLUDE "../Deps/glslang/glslang/Public/ShaderLang.h"
// # define GLSLANG_RESOURCES_INCLUDE "../Deps/glslang/StandAlone/ResourceLimits.h"
// # define GLSLANG_DIRSTACK_INCLUDE "../Deps/glslang/StandAlone/DirStackFileIncluder.h"
// # define GLSLANG_SPV_INCLDUE "../Deps/glslang/SPIRV/GlslangToSpv.h"

# ifdef __APPLE__
#  define VULKAN_INCLUDE "../Deps/vulkansdk-macos-1.1.85.0/macOS/include/vulkan/vulkan.h"
# else
#  define VULKAN_INCLUDE <vulkan/vulkan.h>
# endif

#endif