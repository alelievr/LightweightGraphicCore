# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amerelo <amerelo@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2014/07/15 15:13:38 by alelievr          #+#    #+#              #
#    Updated: 2019/03/23 18:43:44 by amerelo          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#################
##  VARIABLES  ##
#################

#	Sources
SRCDIR		=	Sources
SOURCES		=	Core/Application.cpp \
				Core/Transform.cpp \
				Core/GameObject.cpp \
				Core/Hierarchy.cpp \
				Core/EventSystem.cpp \
				Core/MaterialTable.cpp \
				Core/Mesh.cpp \
				Core/ShaderCache.cpp \
				Core/Object.cpp \
				Core/Time.cpp \
				Core/PrimitiveMeshFactory.cpp \
				Core/ComputeDispatcher.cpp \
				Core/TextureTable.cpp \
				Core/Components/Component.cpp \
				Core/Components/Camera.cpp \
				Core/Components/Renderer.cpp \
				Core/Components/ProceduralRenderer.cpp \
				Core/Components/MeshRenderer.cpp \
				Core/Components/FreeCameraControls.cpp \
				Core/Components/Light.cpp \
				Core/Components/ImGUIPanel.cpp \
				Core/Components/Rotator.cpp \
				Core/Components/Movator.cpp \
				Core/Components/Activator.cpp \
				Core/ComputeDispatcher.cpp \
				Core/ShaderCache.cpp \
				Core/Object.cpp \
				Core/Texture2DAtlas.cpp \
				Core/NodeTree.cpp \
				Core/PrimitiveMeshFactory.cpp \
				Core/Rendering/ForwardRenderPipeline.cpp \
				Core/Rendering/RenderTarget.cpp \
				Core/Rendering/RenderPipeline.cpp \
				Core/Rendering/RenderPipelineManager.cpp \
				Core/Rendering/DefaultRenderQueue.cpp \
				Core/Shaders/ShaderProgram.cpp \
				Core/Shaders/ShaderSource.cpp \
				Core/Shaders/BuiltinShaders.cpp \
				Core/Shaders/ShaderBindingTable.cpp \
				Core/Vulkan/CommandBufferPool.cpp \
				Core/Vulkan/Material.cpp \
				Core/Vulkan/RenderPass.cpp \
				Core/Vulkan/SwapChain.cpp \
				Core/Vulkan/Vk.cpp \
				Core/Vulkan/VkExt.cpp \
				Core/Vulkan/VulkanInstance.cpp \
				Core/Vulkan/VulkanSurface.cpp \
				Core/Vulkan/ProfilingSample.cpp \
				Core/Vulkan/ComputeShader.cpp \
				Core/Vulkan/DescriptorSet.cpp \
				Core/Vulkan/MaterialStates.cpp \
				Core/Textures/Texture.cpp \
				Core/Textures/Texture2D.cpp \
				Core/Textures/Texture2DArray.cpp \
				Core/Textures/TextureCube.cpp \
				Core/Textures/TextureCubeArray.cpp \
				Core/Textures/Texture3D.cpp \
				Core/Gizmos/GizmoBase.cpp \
				Core/Gizmos/Line.cpp \
				Core/Gizmos/Ray.cpp \
				Core/Gizmos/Frustum.cpp \
				Core/Gizmos/Circle.cpp \
				Core/Gizmos/Cone.cpp \
				Core/Gizmos/Arrow.cpp \
				Core/Gizmos/Position.cpp \
				Core/Handles/Selection.cpp \
				Core/Handles/HandleManager.cpp \
				Core/Handles/PositionHandle.cpp \
				Core/Handles/Tools.cpp \
				Core/Handles/Slider1DHandle.cpp \
				Core/Handles/HandleControl.cpp \
				Core/Handles/HandleUtils.cpp \
				Core/ImGUIWrapper.cpp \
				Core/ModelLoader.cpp \
				Utils/Bounds.cpp \
				Utils/Color.cpp \
				Utils/Random.cpp \
				Utils/Rect.cpp \
				Utils/Math.cpp \
				Utils/Vector.cpp \
				Utils/Utils.cpp \

#	Objects
OBJDIR		=	Objects

#	Variables
LIBFT		=	2	#1 or 0 to include the libft / 2 for autodetct
DEBUGLEVEL	=	0	#can be 0 for no debug 1 for or 2 for harder debug
					#Warrning: non null debuglevel will disable optlevel
OPTLEVEL	=	1	#same than debuglevel
					#Warrning: non null optlevel will disable debuglevel
CPPVERSION	=	c++1z
#For simpler and faster use, use commnd line variables DEBUG and OPTI:
#Example $> make DEBUG=2 will set debuglevel to 2

#	Includes
INCDIRS		=	Sources/ Deps/vulkansdk-macos-1.1.85.0/macOS/include/ Deps/assimp/include

#	Libraries
LIBDIRS		=	Deps/glfw/src Deps/glslang/install/lib Deps/glm
LDLIBS		=

GLFWLIB     =   Deps/glfw/src/libglfw3.a
STBLIB      =   Deps/stb/stb.h
GLMLIB      =   Deps/glm/glm
GLSLANGLIB	=	Deps/glslang/build/StandAlone/glslangValidator
IMGUILIB    =   Deps/imgui/libImGUI.a
SPIRV_CROSSLIB	=	Deps/SPIRV-Cross/libspirv-cross.a
ASSIMPLIB	=	Deps/assimp/lib/libassimp.a

#	Output
NAME		=	libLWGC.a

#	Compiler
CFLAGS		=	-Wall -Wextra -pedantic -fPIC
CPROTECTION	=	-z execstack -fno-stack-protector

DEBUGFLAGS1	=	-ggdb -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls -O0 -DDEBUG
DEBUGFLAGS2	=	-fsanitize-memory-track-origins=2 -DDEBUG2
OPTFLAGS1	=	-funroll-loops -O2
OPTFLAGS2	=	-pipe -funroll-loops -Ofast
MYCC		=	clang++

#################
##  COLORS     ##
#################
CPREFIX		=	"\033[38;5;"
BGPREFIX	=	"\033[48;5;"
CCLEAR		=	"\033[0m"
CLINK_T		=	$(CPREFIX)"129m"
CLINK		=	$(CPREFIX)"93m"
COBJ_T		=	$(CPREFIX)"119m"
COBJ		=	$(CPREFIX)"113m"
CCLEAN_T	=	$(CPREFIX)"9m"
CCLEAN		=	$(CPREFIX)"166m"
CRUN_T		=	$(CPREFIX)"198m"
CRUN		=	$(CPREFIX)"163m"
CNORM_T		=	"226m"
CNORM_ERR	=	"196m"
CNORM_WARN	=	"202m"
CNORM_OK	=	"231m"

#################
##  OS/PROC    ##
#################

OS			:=	$(shell uname -s)
PROC		:=	$(shell uname -p)
DEBUGFLAGS	=
LINKDEBUG	=
OPTFLAGS	=
#COMPILATION	=

ifeq "$(OS)" "Windows_NT"
endif
ifeq "$(OS)" "Linux"
	LDLIBS		+=
	DEBUGFLAGS	+=
ifndef VULKAN_SDK
	DOWNLOAD_VULKAN = echo error Vulkan SDK not found, please install it: https://vulkan.lunarg.com/sdk/home\#linux
endif
	INCDIRS		+= $(VULKAN_SDK)/include
	VULKAN		= $(VULKAN_SDK)/lib/libvulkan.dylib
endif
ifeq "$(OS)" "Darwin"
	CFLAGS			+= "-ferror-limit=999"
	MoltenTar		= moltenVK.tar.gz
	MoltentUrl		= https://sdk.lunarg.com/sdk/download/1.1.101.0/mac/vulkansdk-macos-1.1.101.0.tar.gz?Human=true
	DOWNLOAD_VULKAN = curl -o $(MoltenTar) $(MoltentUrl) && tar -xf $(MoltenTar) -C Deps/
	VULKAN_SDK		= $(shell pwd)/Deps/vulkansdk-macos-1.1.101.0/macOS
	LD_LIBRARY_PATH	= $(VULKAN_SDK)/lib
	VK_ICD_FILENAMES= $(VULKAN_SDK)/etc/vulkan/icd.d/MoltenVK_icd.json
	INCDIRS			+= $(VULKAN_SDK)/include
	VULKAN			= $(VULKAN_SDK)/lib/libvulkan.dylib
endif

#################
##  AUTO       ##
#################

NASM		=	nasm
OBJS		=	$(patsubst %.c,%.o, $(filter %.c, $(SOURCES))) \
				$(patsubst %.cpp,%.o, $(filter %.cpp, $(SOURCES))) \
				$(patsubst %.s,%.o, $(filter %.s, $(SOURCES)))
OBJ			=	$(addprefix $(OBJDIR)/,$(notdir $(OBJS)))
NORME		=	**/*.[ch]
VPATH		+=	$(dir $(addprefix $(SRCDIR)/,$(SOURCES)))
INCFILES	=	$(foreach inc, $(INCDIRS), $(wildcard $(inc)/*.h))
CPPFLAGS	=	$(addprefix -I,$(INCDIRS))
LDFLAGS		=	$(addprefix -L,$(LIBDIRS))
LINKER		=	ar
LINKFLAGS	=	rc

disp_indent	=	for I in `seq 1 $(MAKELEVEL)`; do \
					test "$(MAKELEVEL)" '!=' '0' && printf "\t"; \
				done
color_exec	=	$(call disp_indent); \
				echo $(1)➤ $(3)$(2)"\n"$(strip $(4))$(CCLEAR);$(4)
color_exec_t=	$(call disp_indent); \
				echo $(1)➤ $(strip $(3))$(2)$(CCLEAR);$(3)

ifneq ($(filter 1,$(strip $(DEBUGLEVEL)) ${DEBUG}),)
	OPTLEVEL = 0
	OPTI = 0
	DEBUGFLAGS += $(DEBUGFLAGS1)
endif
ifneq ($(filter 2,$(strip $(DEBUGLEVEL)) ${DEBUG}),)
	OPTLEVEL = 0
	OPTI = 0
	DEBUGFLAGS += $(DEBUGFLAGS1)
	LINKDEBUG += $(DEBUGFLAGS1) $(DEBUGFLAGS2)
	export ASAN_OPTIONS=check_initialization_order=1
endif

ifneq ($(filter 1,$(strip $(OPTLEVEL)) ${OPTI}),)
	DEBUGFLAGS = ""
	OPTFLAGS = $(OPTFLAGS1)
endif
ifneq ($(filter 2,$(strip $(OPTLEVEL)) ${OPTI}),)
	DEBUGFLAGS = ""
	OPTFLAGS = $(OPTFLAGS1) $(OPTFLAGS2)
endif

ifndef $(CXX)
	CXX = clang++
endif

ifeq "$(strip $(LIBFT))" "2"
ifneq ($(wildcard ./libft),)
	LIBDIRS += " -Llibft"
	LDLIBS += " -lft"
	INCDIRS += " -Ilibft"
endif
endif

#################
##  TARGETS    ##
#################

#	First target
all: $(VULKAN) $(GLFWLIB) $(OBJLIB) $(GLMLIB) $(IMGUILIB) $(SPIRV_CROSSLIB) $(GLSLANGLIB) $(STBLIB) $(ASSIMPLIB) $(NAME)

$(GLMLIB):
	@git submodule init
	@git submodule update

$(STBLIB):
	@git submodule init
	@git submodule update

$(GLFWLIB):
	@git submodule init
	@git submodule update
	#@cd Deps/glfw && VULKAN_SDK=$(VULKAN_SDK) cmake -DVULKAN_STATIC_LIBRARY=0 -DCMAKE_FIND_FRAMEWORK=${VULKAN_SDK}/../ -DVULKAN_INCLUDE_DIR=${VULKAN_SDK}/include -DVULKAN_LIBRARY=${VULKAN_SDK}/lib/libvulkan.dylib -DGLFW_BUILD_EXAMPLES=OFF . && $(MAKE) -j 4
	@cd Deps/glfw && VULKAN_SDK=$(VULKAN_SDK) cmake -DVULKAN_STATIC_LIBRARY=0 . && $(MAKE) -j 4

$(GLSLANGLIB):
	@git submodule init
	@git submodule update
	@cd Deps/glslang && mkdir -p build && cd build && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="$(pwd)/../install" .. && $(MAKE) -j 4

$(IMGUILIB):
	@git submodule init
	@git submodule update
	@$(MAKE) -f ImGUI.Makefile -j4

$(SPIRV_CROSSLIB):
	@git submodule update --init
	@cd Deps/SPIRV-Cross/ && make -j4

$(ASSIMPLIB):
	@git submodule update --init
	@cd Deps/assimp && cmake . -DBUILD_SHARED_LIBS=OFF && make -j4

$(VULKAN):
	@$(DOWNLOAD_VULKAN)

#	Linking
$(NAME): $(OBJ)
	@$(if $(findstring libft,$(LDLIBS)),$(call color_exec_t,$(CCLEAR),$(CCLEAR),\
		make -j 4 -C libft))
	@$(call color_exec,$(CLINK_T),$(CLINK),"Link of $(NAME):",\
		$(LINKER) $(LINKFLAGS) $@ $^)

$(OBJDIR)/%.o: %.cpp $(INCFILES)
	@mkdir -p $(OBJDIR)
	@$(call color_exec,$(COBJ_T),$(COBJ),"Object: $@",\
		$(CXX) -std=$(CPPVERSION) $(CFLAGS) $(OPTFLAGS) $(DEBUGFLAGS) $(CPPFLAGS) -o $@ -c $<)

#	Objects compilation
$(OBJDIR)/%.o: %.c $(INCFILES)
	@mkdir -p $(OBJDIR)
	@$(call color_exec,$(COBJ_T),$(COBJ),"Object: $@",\
		$(MYCC) $(CFLAGS) $(OPTFLAGS) $(DEBUGFLAGS) $(CPPFLAGS) -o $@ -c $<)

$(OBJDIR)/%.o: %.s
	@mkdir -p $(OBJDIR)
	@$(call color_exec,$(COBJ_T),$(COBJ),"Object: $@",\
		$(NASM) -f macho64 -o $@ $<)

#	Removing objects
clean:
	@$(call color_exec,$(CCLEAN_T),$(CCLEAN),"Clean:",\
		$(RM) $(OBJ))
	@rm -rf $(OBJDIR)

#	Removing objects and exe
fclean: clean
	@$(call color_exec,$(CCLEAN_T),$(CCLEAN),"Fclean:",\
		$(RM) $(NAME))

examples:
	@$(MAKE) -C examples

#	All removing then compiling
re: fclean
	@$(MAKE) all

f:	all run

#	Checking norme
norme:
	@norminette $(NORME) | sed "s/Norme/[38;5;$(CNORM_T)➤ [38;5;$(CNORM_OK)Norme/g;s/Warning/[0;$(CNORM_WARN)Warning/g;s/Error/[0;$(CNORM_ERR)Error/g"

run: $(NAME)
	@echo $(CRUN_T)"➤ "$(CRUN)"./$(NAME) ${ARGS}\033[0m"
	@./$(NAME) ${ARGS}

codesize:
	@cat $(NORME) |grep -v '/\*' |wc -l

functions: $(NAME)
	@nm $(NAME) | grep U

coffee:
	@clear
	@echo ""
	@echo "                   ("
	@echo "	                     )     ("
	@echo "               ___...(-------)-....___"
	@echo '           .-""       )    (          ""-.'
	@echo "      .-''''|-._             )         _.-|"
	@echo '     /  .--.|   `""---...........---""`   |'
	@echo "    /  /    |                             |"
	@echo "    |  |    |                             |"
	@echo "     \  \   |                             |"
	@echo "      '\ '\ |                             |"
	@echo "        '\ '|                             |"
	@echo "        _/ /\                             /"
	@echo "       (__/  \                           /"
	@echo '    _..---""` \                         /`""---.._'
	@echo " .-'           \                       /          '-."
	@echo ":               '-.__             __.-'              :"
	@echo ':                  ) ""---...---"" (                :'
	@echo "\'._                '"--...___...--"'              _.'"
	@echo '   \""--..__                              __..--""/'
	@echo "     '._     """----.....______.....----"""         _.'"
	@echo '         ""--..,,_____            _____,,..--"""'''
	@echo '                      """------"""'
	@sleep 0.5
	@clear
	@echo ""
	@echo "                 ("
	@echo "	                  )      ("
	@echo "               ___..(.------)--....___"
	@echo '           .-""       )   (           ""-.'
	@echo "      .-''''|-._      (       )        _.-|"
	@echo '     /  .--.|   `""---...........---""`   |'
	@echo "    /  /    |                             |"
	@echo "    |  |    |                             |"
	@echo "     \  \   |                             |"
	@echo "      '\ '\ |                             |"
	@echo "        '\ '|                             |"
	@echo "        _/ /\                             /"
	@echo "       (__/  \                           /"
	@echo '    _..---""` \                         /`""---.._'
	@echo " .-'           \                       /          '-."
	@echo ":               '-.__             __.-'              :"
	@echo ':                  ) ""---...---"" (                :'
	@echo "\'._                '"--...___...--"'              _.'"
	@echo '   \""--..__                              __..--""/'
	@echo "     '._     """----.....______.....----"""         _.'"
	@echo '         ""--..,,_____            _____,,..--"""'''
	@echo '                      """------"""'
	@sleep 0.5
	@clear
	@echo ""
	@echo "               ("
	@echo "	                  )     ("
	@echo "               ___..(.------)--....___"
	@echo '           .-""      )    (           ""-.'
	@echo "      .-''''|-._      (       )        _.-|"
	@echo '     /  .--.|   `""---...........---""`   |'
	@echo "    /  /    |                             |"
	@echo "    |  |    |                             |"
	@echo "     \  \   |                             |"
	@echo "      '\ '\ |                             |"
	@echo "        '\ '|                             |"
	@echo "        _/ /\                             /"
	@echo "       (__/  \                           /"
	@echo '    _..---""` \                         /`""---.._'
	@echo " .-'           \                       /          '-."
	@echo ":               '-.__             __.-'              :"
	@echo ':                  ) ""---...---"" (                :'
	@echo "\'._                '"--...___...--"'              _.'"
	@echo '   \""--..__                              __..--""/'
	@echo "     '._     """----.....______.....----"""         _.'"
	@echo '         ""--..,,_____            _____,,..--"""'''
	@echo '                      """------"""'
	@sleep 0.5
	@clear
	@echo ""
	@echo "             (         ) "
	@echo "	              )        ("
	@echo "               ___)...----)----....___"
	@echo '           .-""      )    (           ""-.'
	@echo "      .-''''|-._      (       )        _.-|"
	@echo '     /  .--.|   `""---...........---""`   |'
	@echo "    /  /    |                             |"
	@echo "    |  |    |                             |"
	@echo "     \  \   |                             |"
	@echo "      '\ '\ |                             |"
	@echo "        '\ '|                             |"
	@echo "        _/ /\                             /"
	@echo "       (__/  \                           /"
	@echo '    _..---""` \                         /`""---.._'
	@echo " .-'           \                       /          '-."
	@echo ":               '-.__             __.-'              :"
	@echo ':                  ) ""---...---"" (                :'
	@echo "\'._                '"--...___...--"'              _.'"
	@echo '   \""--..__                              __..--""/'
	@echo "     '._     """----.....______.....----"""         _.'"
	@echo '         ""--..,,_____            _____,,..--"""'''
	@echo '                      """------"""'

.PHONY: all clean fclean re norme codesize examples
