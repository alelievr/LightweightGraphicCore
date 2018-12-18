#ifndef INCLUDEDEPS_HPP
# define INCLUDEDEPS_HPP

# define GLM_ENABLE_EXPERIMENTAL
# define GLM_FORCE_RADIANS
# define GLM_FORCE_DEPTH_ZERO_TO_ONE

# define GLM_INCLUDE "../Deps/glm/glm/glm.hpp"
# define GLM_INCLUDE_QUATERNION "../Deps/glm/glm/gtx/quaternion.hpp"
# define GLM_INCLUDE_QUATERNION2 "../Deps/glm/glm/gtc/quaternion.hpp"
# define GLM_INCLUDE_MATRIX_TRANSFORM "../Deps/glm/glm/gtc/matrix_transform.hpp"
# define GLM_INCLUDE_STRING_CAST "../Deps/glm/glm/gtx/string_cast.hpp"
# define GLFW_INCLUDE "../Deps/glfw/include/GLFW/glfw3.h"
# define IMGUI_INCLUDE "../Deps/imgui/imgui.h"
# define IMGUI_GLFW_INCLUDE "../Deps/imgui/examples/imgui_impl_glfw.h"
# define IMGUI_VULKAN_INCLUDE "../Deps/imgui/examples/imgui_impl_vulkan.h"
# define STB_INCLUDE "../Deps/stb/stb.h"
# define STB_INCLUDE_IMAGE "../Deps/stb/stb_image.h"
# define SPIRV_CROSS_INCLUDE "../Deps/SPIRV-Cross/spirv_glsl.hpp"
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
