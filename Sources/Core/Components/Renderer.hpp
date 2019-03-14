#pragma once

#include <iostream>
#include <string>

#include "Core/Object.hpp"
#include "Core/Mesh.hpp"
#include "Core/Vulkan/Material.hpp"
#include "Core/Vulkan/UniformBuffer.hpp"
#include "Component.hpp"
#include "Core/Rendering/PipelineCommandBuffer.hpp"

namespace LWGC
{
	class		Renderer : public Object, public Component, public PipelineCommandBuffer
	{
		private:
			struct LWGC_PerObject
			{
				glm::mat4	model;
			};

			LWGC_PerObject		_perObject;
			ComponentIndex		_renderContextIndex;
			UniformBuffer		_uniformModelBuffer;
			VkDescriptorSet		_descriptorSet;

			static VkDescriptorSetLayout	_descriptorSetLayout;

			static void		CreateGraphicDescriptorSetLayout(void) noexcept;

			virtual void	CreateDescriptorSet(void);

		protected:
			Material *	_material;

			void			Initialize(void) noexcept override;
			virtual void	RecordDrawCommand(VkCommandBuffer cmd) noexcept = 0;
			virtual void	UpdateUniformData(void);
			void			Update(void) noexcept override;

		public:
			Renderer(void);
			Renderer(Material * material);
			Renderer(const Renderer &) = delete;

			virtual ~Renderer(void);

			Renderer &	operator=(Renderer const & src) = delete;

			virtual Bounds	GetBounds(void) noexcept;

			void	OnEnable(void) noexcept override;
			void	OnDisable(void) noexcept override;

			void	RecordCommands(VkCommandBuffer cmd) override;

			Material *	GetMaterial(void) const;
			void	SetMaterial(Material * tmp);

			VkDescriptorSet		GetDescriptorSet(void);

			virtual uint32_t	GetType(void) const noexcept override = 0;
	};

	std::ostream &	operator<<(std::ostream & o, Renderer const & r);
}
