#include "RenderTarget.hpp"


RenderTarget::RenderTarget(void)
{
	std::cout << "Default constructor of RenderTarget called" << std::endl;
	this->_framebuffer = ;
	this->_attachmentIds = NULL;
	this->_name = "";
	this->_size = ;
	this->_enabledAttachments = 0;
}

RenderTarget::RenderTarget(RenderTarget const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

RenderTarget::~RenderTarget(void)
{
	std::cout << "Destructor of RenderTarget called" << std::endl;
}

void		RenderTarget::AddAttachment(const FramebufferAttachment fba)
{
	
}

void		RenderTarget::AddAttachment(const FramebufferAttachment fba, GLuint attachmentId)
{
	
}

void		RenderTarget::RemoveAttachment(const FramebufferAttachment fba)
{
	
}

void		RenderTarget::Update(void)
{
	
}

RenderTarget &	RenderTarget::operator=(RenderTarget const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
		this->_framebuffer = src.GetFramebuffer();
		this->_attachmentIds = src.GetAttachmentIds();
		this->_name = src.GetName();
		this->_size = src.GetSize();
		this->_enabledAttachments = src.GetEnabledAttachments();
	}
	return (*this);
}

GLuint		RenderTarget::GetFramebuffer(void) const { return (this->_framebuffer); }
void		RenderTarget::SetFramebuffer(GLuint tmp) { this->_framebuffer = tmp; }

GLuint *		RenderTarget::GetAttachmentIds(void) const { return (this->_attachmentIds); }
void		RenderTarget::SetAttachmentIds(GLuint * tmp) { this->_attachmentIds = tmp; }

std::string		RenderTarget::GetName(void) const { return (this->_name); }
void		RenderTarget::SetName(std::string tmp) { this->_name = tmp; }

glm::vec2		RenderTarget::GetSize(void) const { return (this->_size); }
void		RenderTarget::SetSize(glm::vec2 tmp) { this->_size = tmp; }

int		RenderTarget::GetEnabledAttachments(void) const { return (this->_enabledAttachments); }
void		RenderTarget::SetEnabledAttachments(int tmp) { this->_enabledAttachments = tmp; }

std::ostream &	operator<<(std::ostream & o, RenderTarget const & r)
{
	o << r.GetName() << std::endl;
	(void)r;
	return (o);
}
