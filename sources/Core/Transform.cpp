#include "Transform.hpp"


Transform::Transform(void)
{
	std::cout << "Default constructor of Transform called" << std::endl;
	this->_parent = NULL;
	this->_childs = ;
	this->_position = ;
	this->_rotation = ;
	this->_scale = ;
	this->_up = ;
	this->_down = ;
	this->_right = ;
	this->_left = ;
	this->_forward = ;
	this->_back = ;
	this->_eulerAngles = ;
}

Transform::Transform(Transform const & src)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
}

Transform::~Transform(void)
{
}

void		Transform::Rotate(const glm::vec3 & eulerAngles)
{
	
}

void		Transform::Rotate(const float xAngle, const float yAngle, const float zAngle)
{
	
}

void		Transform::RotateAround(const glm::vec3 & point, const glm::vec3 & axis, const float angle)
{
	
}

size_t		Transform::GetChildCount(void)
{
	
}

bool		Transform::IsChildOf(const Transform & t)
{
	
}

Transform		Transform::GetChildAt(const int index) const
{
	
}

void		Transform::LookAt(const int index)
{
	
}

glm::vec3		Transform::TransformDirection(const glm::vec3 & direction)
{
	
}

glm::vec3		Transform::TransformDirection(const float x, const float y, const float z)
{
	
}

glm::vec3		Transform::TransformPoint(const glm::vec3 & position)
{
	
}

glm::vec3		Transform::TransformPoint(const float x, const float y, const float z)
{
	
}

glm::vec3		Transform::TransformDirection(const glm::vec3 & direction)
{
	
}

void		Transform::Translate(const glm::vec3 & translation)
{
	
}

Transform &		Transform::GetRoot(void)
{
	
}


Transform &	Transform::operator=(Transform const & src)
{
	std::cout << "Assignment operator called" << std::endl;

	if (this != &src) {
		this->_parent = src.getParent();
		this->_childs = src.getChilds();
		this->_position = src.getPosition();
		this->_rotation = src.getRotation();
		this->_scale = src.getScale();
		this->_up = src.getUp();
		this->_down = src.getDown();
		this->_right = src.getRight();
		this->_left = src.getLeft();
		this->_forward = src.getForward();
		this->_back = src.getBack();
		this->_eulerAngles = src.getEulerAngles();
	}
	return (*this);
}

Transform *		Transform::getParent(void) const { return (this->_parent); }
void			Transform::setParent(Transform * tmp) { this->_parent = tmp; }

std::list< Transform >		Transform::getChilds(void) const { return (this->_childs); }
void			Transform::setChilds(std::list< Transform > tmp) { this->_childs = tmp; }

glm::vec3		Transform::getPosition(void) const { return (this->_position); }
void			Transform::setPosition(glm::vec3 tmp) { this->_position = tmp; }

glm::quat		Transform::getRotation(void) const { return (this->_rotation); }
void			Transform::setRotation(glm::quat tmp) { this->_rotation = tmp; }

glm::vec3		Transform::getScale(void) const { return (this->_scale); }
void			Transform::setScale(glm::vec3 tmp) { this->_scale = tmp; }

glm::vec3		Transform::getUp(void) const { return (this->_up); }
void			Transform::setUp(glm::vec3 tmp) { this->_up = tmp; }

glm::vec3		Transform::getDown(void) const { return (this->_down); }
void			Transform::setDown(glm::vec3 tmp) { this->_down = tmp; }

glm::vec3		Transform::getRight(void) const { return (this->_right); }
void			Transform::setRight(glm:vec3 tmp) { this->_right = tmp; }

glm::vec3		Transform::getLeft(void) const { return (this->_left); }
void			Transform::setLeft(glm::vec3 tmp) { this->_left = tmp; }

glm::vec3		Transform::getForward(void) const { return (this->_forward); }
void			Transform::setForward(glm::vec3 tmp) { this->_forward = tmp; }

glm::vec3		Transform::getBack(void) const { return (this->_back); }
void			Transform::setBack(glm::vec3 tmp) { this->_back = tmp; }

glm::vec3		Transform::getEulerAngles(void) const { return (this->_eulerAngles); }
void			Transform::setEulerAngles(glm::vec3 tmp) { this->_eulerAngles = tmp; }

std::ostream &	operator<<(std::ostream & o, Transform const & r)
{
	glm::vec3	pos = r.GetPosition();
	o << "(" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;
	(void)r;
	return (o);
}
