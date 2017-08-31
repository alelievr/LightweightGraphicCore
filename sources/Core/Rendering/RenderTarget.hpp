#ifndef RENDERTARGET_HPP
# define RENDERTARGET_HPP
# include <iostream>
# include <string>

class		RenderTarget
{
	private:
		GLuint	_framebuffer;
		GLuint *	_attachmentIds;
		std::string	_name;
		glm::vec2	_size;
		int	_enabledAttachments;


	public:
		RenderTarget();
		RenderTarget(const RenderTarget&);
		virtual ~RenderTarget(void);

		RenderTarget &	operator=(RenderTarget const & src);

		void	AddAttachment(const FramebufferAttachment fba);

		void	AddAttachment(const FramebufferAttachment fba, GLuint attachmentId);

		void	RemoveAttachment(const FramebufferAttachment fba);

		void	Update(void);

		GLuint	getFramebuffer(void) const;
		void	setFramebuffer(GLuint tmp);
		
		GLuint *	getAttachmentIds(void) const;
		void	setAttachmentIds(GLuint * tmp);
		
		std::string	getName(void) const;
		void	setName(std::string tmp);
		
		glm::vec2	getSize(void) const;
		void	setSize(glm::vec2 tmp);
		
		int	getEnabledAttachments(void) const;
		void	setEnabledAttachments(int tmp);
};

std::ostream &	operator<<(std::ostream & o, RenderTarget const & r);

#endif
