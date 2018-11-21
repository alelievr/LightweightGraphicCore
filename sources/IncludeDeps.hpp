#ifndef INCLUDEDEPS_HPP
# define INCLUDEDEPS_HPP

# define GLM_ENABLE_EXPERIMENTAL

# define GLM_INCLUDE "../deps/glm/glm/glm.hpp"
# define GLM_INCLUDE_QUATERNION "../deps/glm/glm/gtx/quaternion.hpp"
# define GLM_INCLUDE_MATRIX_TRANSFORM "../deps/glm/glm/gtc/matrix_transform.hpp"
# define GLFW_INCLUDE "../deps/glfw/include/GLFW/glfw3.h"
# define GLSLANG_INCLUDE "../deps/glslang/install/include/glslang/Public/ShaderLang.h"
# define IMGUI_INCLUDE "../deps/imgui/imgui.h"
# define STB_INCLUDE "../deps/stb/stb.h"
# define STB_INCLUDE_IMAGE "../deps/stb/stb_image.h"

# ifdef __APPLE__
#  define VULKAN_INCLUDE "../deps/vulkansdk-macos-1.1.85.0/macOS/include/vulkan/vulkan.h"
# else
#  define VULKAN_INCLUDE <vulkan/vulkan.h>
# endif

#endif
