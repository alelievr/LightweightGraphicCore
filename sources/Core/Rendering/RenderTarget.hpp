#pragma once

#include <iostream>
#include <string>

#include "IncludeDeps.hpp"

#include "FramebufferAttachment.hpp"

#include GLFW_INCLUDE
#include GLM_INCLUDE

namespace LWGC
{
	class		RenderTarget
	{
		private:
			GLuint		_framebuffer;
			GLuint *	_attachmentIds;
			std::string	_name;
			glm::vec2	_size;
			int			_enabledAttachments;

		public:
			RenderTarget(void);
			RenderTarget(const RenderTarget&);
			virtual		~RenderTarget(void);

			RenderTarget &	operator=(RenderTarget const & src);

			void		AddAttachment(const FramebufferAttachment fba);

			void		AddAttachment(const FramebufferAttachment fba, GLuint attachmentId);

			void		RemoveAttachment(const FramebufferAttachment fba);

			void		Update(void);

			GLuint		GetFramebuffer(void) const;
			void		SetFramebuffer(GLuint tmp);
			
			GLuint *	GetAttachmentIds(void) const;
			void		SetAttachmentIds(GLuint * tmp);
			
			std::string	GetName(void) const;
			void		SetName(std::string tmp);
			
			glm::vec2	GetSize(void) const;
			void		SetSize(glm::vec2 tmp);
			
			int			GetEnabledAttachments(void) const;
			void		SetEnabledAttachments(int tmp);
	};

	std::ostream &	operator<<(std::ostream & o, RenderTarget const & r);
}
