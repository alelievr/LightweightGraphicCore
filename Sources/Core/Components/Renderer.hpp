#pragma once

#include <iostream>
#include <string>

#include "Core/Object.hpp"
#include "Core/Mesh.hpp"
#include "Core/Vulkan/Material.hpp"
#include "Core/Vulkan/UniformBuffer.hpp"
#include "Component.hpp"

namespace LWGC
{
	class		Renderer : public Object, public Component
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

			void			RecordDrawCommandBuffer(void);
			virtual void	CreateDescriptorSet(void);

		protected:
			std::shared_ptr< Material >	_material;
			VkCommandBuffer				_drawCommandBuffer;

			virtual void	Initialize(void) noexcept override;
			virtual void	RecordDrawCommand(VkCommandBuffer cmd) noexcept = 0;
			virtual void	UpdateUniformData(void);
			virtual void	Update(void) noexcept override;

		public:
			Renderer(void);
			Renderer(std::shared_ptr< Material > material);
			Renderer(const Renderer &) = delete;

			virtual ~Renderer(void);

			Renderer &	operator=(Renderer const & src) = delete;

			Bounds	GetBounds(void);

			void	OnEnable(void) noexcept override;
			void	OnDisable(void) noexcept override;

			void	CleanupPipeline(void) noexcept;
			void	CreatePipeline(void) noexcept;

			std::shared_ptr< Material >	GetMaterial(void) const;
			void	SetMaterial(std::shared_ptr< Material > tmp);

			VkCommandBuffer		GetDrawCommandBuffer(void) const;
			VkDescriptorSet		GetDescriptorSet(void) const;

			virtual uint32_t	GetType(void) const noexcept override = 0;
	};

	std::ostream &	operator<<(std::ostream & o, Renderer const & r);
}
