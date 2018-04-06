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
		this->_parent = src.GetParent();
		this->_childs = src.GetChilds();
		this->_position = src.GetPosition();
		this->_rotation = src.GetRotation();
		this->_scale = src.GetScale();
		this->_up = src.GetUp();
		this->_down = src.GetDown();
		this->_right = src.GetRight();
		this->_left = src.GetLeft();
		this->_forward = src.GetForward();
		this->_back = src.GetBack();
		this->_eulerAngles = src.GetEulerAngles();
	}
	return (*this);
}

Transform *		Transform::GetParent(void) const { return (this->_parent); }
void			Transform::SetParent(Transform * tmp) { this->_parent = tmp; }

std::list< Transform >		Transform::GetChilds(void) const { return (this->_childs); }
void			Transform::SetChilds(std::list< Transform > tmp) { this->_childs = tmp; }

glm::vec3		Transform::GetPosition(void) const { return (this->_position); }
void			Transform::SetPosition(glm::vec3 tmp) { this->_position = tmp; }

glm::quat		Transform::GetRotation(void) const { return (this->_rotation); }
void			Transform::SetRotation(glm::quat tmp) { this->_rotation = tmp; }

glm::vec3		Transform::GetScale(void) const { return (this->_scale); }
void			Transform::SetScale(glm::vec3 tmp) { this->_scale = tmp; }

glm::vec3		Transform::GetUp(void) const { return (this->_up); }
void			Transform::SetUp(glm::vec3 tmp) { this->_up = tmp; }

glm::vec3		Transform::GetDown(void) const { return (this->_down); }
void			Transform::SetDown(glm::vec3 tmp) { this->_down = tmp; }

glm::vec3		Transform::GetRight(void) const { return (this->_right); }
void			Transform::SetRight(glm::vec3 tmp) { this->_right = tmp; }

glm::vec3		Transform::GetLeft(void) const { return (this->_left); }
void			Transform::SetLeft(glm::vec3 tmp) { this->_left = tmp; }

glm::vec3		Transform::GetForward(void) const { return (this->_forward); }
void			Transform::SetForward(glm::vec3 tmp) { this->_forward = tmp; }

glm::vec3		Transform::GetBack(void) const { return (this->_back); }
void			Transform::SetBack(glm::vec3 tmp) { this->_back = tmp; }

glm::vec3		Transform::GetEulerAngles(void) const { return (this->_eulerAngles); }
void			Transform::SetEulerAngles(glm::vec3 tmp) { this->_eulerAngles = tmp; }

std::ostream &	operator<<(std::ostream & o, Transform const & r)
{
	glm::vec3	pos = r.GetPosition();
	o << "(" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;
	(void)r;
	return (o);
}
