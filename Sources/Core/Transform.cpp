#include "Transform.hpp"

#include <algorithm>

Transform::Transform(void)
{
	this->_parent = nullptr;
	this->_position = glm::vec3(0, 0, 0);
	this->_rotation = glm::quat(0, 0, 0, 0);
	this->_scale = glm::vec3(1, 1, 1);
}

Transform::~Transform(void)
{
}

void		Transform::Rotate(const glm::vec3 & eulerAngles)
{
	_rotation *= glm::quat(eulerAngles);
}

void		Transform::Rotate(const float xAngle, const float yAngle, const float zAngle)
{
	Rotate(glm::vec3(xAngle, yAngle, zAngle));
}

void		Transform::RotateAround(const glm::vec3 & point, const glm::vec3 & axis, const float angle)
{
	std::cout << "RotateAround: TODO" << std::endl;
}

size_t		Transform::GetChildCount(void)
{
	return _childs.size();
}

bool		Transform::IsChildOf(std::shared_ptr< Transform > t)
{
	std::find(t->_childs.begin(), t->_childs.end(), t);
}

std::shared_ptr< Transform >		Transform::GetChildAt(const int index) const
{
	return _childs[index];
}

void		Transform::LookAt(const int index)
{
	
}

glm::vec3		Transform::TransformDirection(const glm::vec3 & direction)
{
	
}

glm::vec3		Transform::TransformDirection(const float x, const float y, const float z)
{
	return TransformDirection(glm::vec3(x, y, z));
}

glm::vec3		Transform::TransformPoint(const glm::vec3 & position)
{
	
}

glm::vec3		Transform::TransformPoint(const float x, const float y, const float z)
{
	return TransformPoint(glm::vec3(x, y, z));
}

void		Transform::Translate(const glm::vec3 & translation)
{
	_position += translation;
	UpdatePositionDatas();
}

std::shared_ptr< Transform >	Transform::GetRoot(void)
{
	
}

void			Transform::UpdatePositionDatas(void) noexcept
{
	UpdateLocalToWorldMatrix();

	for (const auto & child : _childs)
		child->UpdatePositionDatas();
}

void			Transform::UpdateRotationDatas(void) noexcept
{
	UpdateLocalToWorldMatrix();
	
	for (const auto & child : _childs)
		child->UpdateRotationDatas();
}

void			Transform::UpdateScaleDatas(void) noexcept
{
	UpdateLocalToWorldMatrix();
	
	for (const auto & child : _childs)
		child->UpdateScaleDatas();
}

void			Transform::UpdateLocalToWorldMatrix(void) noexcept
{
	_localToWorld = glm::toMat4(_rotation);
}

void			Transform::SetParent(std::shared_ptr< Transform > tmp) { this->_parent = tmp; }

glm::vec3		Transform::GetPosition(void) const { return (this->_position); }
void			Transform::SetPosition(glm::vec3 tmp) { this->_position = tmp; }

glm::quat		Transform::GetRotation(void) const { return (this->_rotation); }
void			Transform::SetRotation(glm::quat tmp) { this->_rotation = tmp; }

glm::vec3		Transform::GetScale(void) const { return (this->_scale); }
void			Transform::SetScale(glm::vec3 tmp) { this->_scale = tmp; }

glm::vec3		Transform::GetUp(void) const { return (glm::vec3(0, 0, 0)); }

glm::vec3		Transform::GetDown(void) const { return (glm::vec3(0, 0, 0)); }

glm::vec3		Transform::GetRight(void) const { return (glm::vec3(0, 0, 0)); }

glm::vec3		Transform::GetLeft(void) const { return (glm::vec3(0, 0, 0)); }

glm::vec3		Transform::GetForward(void) const { return (glm::vec3(0, 0, 0)); }

glm::vec3		Transform::GetBack(void) const { return (glm::vec3(0, 0, 0)); }

glm::vec3		Transform::GetEulerAngles(void) const { return glm::eulerAngles(_rotation); }

std::ostream &	operator<<(std::ostream & o, Transform const & r)
{
	glm::vec3	pos = r.GetPosition();
	o << "(" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;
	(void)r;
	return (o);
}