#include "Camera.hpp"


Camera::Camera(void)
{
	std::cout << "Default constructor of Camera called" << std::endl;
	this->_target = ;
	this->_size = ;
	this->_cameraType = ;
	this->_orthographic = false;
	this->_fov = 0;
	this->_nearPlane = 0;
	this->_farPlane = 0;
}

Camera::Camera(Camera const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

Camera::~Camera(void)
{
	std::cout << "Destructor of Camera called" << std::endl;
}

void		Camera::Render(void)
{
	
}

glm::vec3		Camera::WorldToScreenPoint(glm::vec3 worldPosition)
{
	
}

glm::vec3		Camera::ScreenToWorldPoint(glm::vec3 screenPosition)
{
	
}


Camera &	Camera::operator=(Camera const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
		this->_target = src.getTarget();
		this->_size = src.getSize();
		this->_cameraType = src.getCameraType();
		this->_orthographic = src.getOrthographic();
		this->_fov = src.getFov();
		this->_nearPlane = src.getNearPlane();
		this->_farPlane = src.getFarPlane();
	}
	return (*this);
}

RenderTarget		Camera::getTarget(void) const { return (this->_target); }
void		Camera::setTarget(RenderTarget tmp) { this->_target = tmp; }

glm::vec2		Camera::getSize(void) const { return (this->_size); }
void		Camera::setSize(glm::vec2 tmp) { this->_size = tmp; }

CameraType		Camera::getCameraType(void) const { return (this->_cameraType); }
void		Camera::setCameraType(CameraType tmp) { this->_cameraType = tmp; }

bool		Camera::getOrthographic(void) const { return (this->_orthographic); }
void		Camera::setOrthographic(bool tmp) { this->_orthographic = tmp; }

float		Camera::getFov(void) const { return (this->_fov); }
void		Camera::setFov(float tmp) { this->_fov = tmp; }

float		Camera::getNearPlane(void) const { return (this->_nearPlane); }
void		Camera::setNearPlane(float tmp) { this->_nearPlane = tmp; }

float		Camera::getFarPlane(void) const { return (this->_farPlane); }
void		Camera::setFarPlane(float tmp) { this->_farPlane = tmp; }

std::ostream &	operator<<(std::ostream & o, Camera const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
