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
		this->_framebuffer = src.getFramebuffer();
		this->_attachmentIds = src.getAttachmentIds();
		this->_name = src.getName();
		this->_size = src.getSize();
		this->_enabledAttachments = src.getEnabledAttachments();
	}
	return (*this);
}

GLuint		RenderTarget::getFramebuffer(void) const { return (this->_framebuffer); }
void		RenderTarget::setFramebuffer(GLuint tmp) { this->_framebuffer = tmp; }

GLuint *		RenderTarget::getAttachmentIds(void) const { return (this->_attachmentIds); }
void		RenderTarget::setAttachmentIds(GLuint * tmp) { this->_attachmentIds = tmp; }

std::string		RenderTarget::getName(void) const { return (this->_name); }
void		RenderTarget::setName(std::string tmp) { this->_name = tmp; }

glm::vec2		RenderTarget::getSize(void) const { return (this->_size); }
void		RenderTarget::setSize(glm::vec2 tmp) { this->_size = tmp; }

int		RenderTarget::getEnabledAttachments(void) const { return (this->_enabledAttachments); }
void		RenderTarget::setEnabledAttachments(int tmp) { this->_enabledAttachments = tmp; }

std::ostream &	operator<<(std::ostream & o, RenderTarget const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
