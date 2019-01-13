#include "Transform.hpp"

#include <algorithm>
#include "Utils/Math.hpp"
#include "Utils/Vector.hpp"

using namespace LWGC;

Transform::Transform(GameObject * go) : _gameObject(go)
{
	this->_parent = nullptr;
	this->_position = glm::vec3(0, 0, 0);
	this->_rotation = glm::quat(1, 0, 0, 0);
	this->_scale = glm::vec3(1, 1, 1);
	// TODO: replace pitch, yaw and roll by quaternions
	this->_yaw = 0;
	this->_pitch = 0;
	this->_roll = 0;

	UpdateLocalToWorldMatrix();
}

Transform::~Transform(void)
{
}

void		Transform::Rotate(const glm::vec3 & angleInradians)
{
	_yaw += angleInradians.x;
	_pitch += angleInradians.y;
	_roll += angleInradians.z;

	// FPS camera:  RotationX(pitch) * RotationY(yaw)
	glm::quat qPitch = glm::angleAxis(_pitch, glm::vec3(1, 0, 0));
	glm::quat qYaw = glm::angleAxis(_yaw, glm::vec3(0, 1, 0));

	// std::cout << "_yaw: " << _yaw << ", pitch: " << _pitch << ", roll: " << _roll << "\n";

	// For a FPS camera we can omit roll
	_rotation = qPitch * qYaw;
	_rotation = glm::normalize(_rotation);
	//   glm::mat4 rotate = glm::mat4_cast(orientation);

	UpdateRotationDatas();
}

void		Transform::RotateAxis(float angle, const glm::vec3 & axis)
{
	_rotation = glm::rotate(_rotation, angle, axis);
	_rotation = glm::normalize(_rotation);

	UpdateRotationDatas();
}

void		Transform::Rotate(const float xAngle, const float yAngle, const float zAngle)
{
	Rotate(glm::vec3(xAngle, yAngle, zAngle) * Math::DegToRad);
}

void		Transform::RotateAround(const glm::vec3 & point, const glm::vec3 & axis, const float angle)
{
	(void)point, (void)axis, (void)angle;

	std::cout << "RotateAround: TODO" << std::endl;

	UpdateRotationDatas();
}

size_t		Transform::GetChildCount(void)
{
	return _childs.size();
}

bool		Transform::IsChildOf(Transform * t)
{
	return std::find(t->_childs.begin(), t->_childs.end(), t) != t->_childs.end();
}

Transform *	Transform::GetChildAt(const int index) const
{
	return _childs[index];
}

std::vector< Transform * > &	Transform::GetChilds(void)
{
	return _childs;
}

void		Transform::LookAt(const glm::vec3 & direction, const glm::vec3 & up)
{
	_rotation = glm::quatLookAt(direction, up);
	UpdateLocalToWorldMatrix();
}

glm::vec3		Transform::TransformDirection(const glm::vec3 & direction)
{
	return glm::vec4(direction, 0) * _localToWorld;
}

glm::vec3		Transform::TransformDirection(const float x, const float y, const float z)
{
	return TransformDirection(glm::vec3(x, y, z));
}

glm::vec3		Transform::TransformPoint(const glm::vec3 & position)
{
	return glm::vec4(position, 1) * _localToWorld;
}

glm::vec3		Transform::TransformPoint(const float x, const float y, const float z)
{
	return TransformPoint(glm::vec3(x, y, z));
}

void			Transform::Translate(const glm::vec3 & translation)
{
	_position += translation;
	UpdatePositionDatas();
}

void			Transform::Scale(const glm::vec3 & scaleFactor)
{
	_scale *= scaleFactor;
	UpdateScaleDatas();
}

Transform *		Transform::GetRoot(void)
{
	Transform *	tmp;

	if (_parent == nullptr)
		return this;

	tmp = _parent;
	while (tmp->_parent != nullptr)
		tmp = tmp->_parent;
	return tmp;
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
	_localToWorld = glm::translate(glm::mat4(1.0f), _position) * glm::toMat4(_rotation) * glm::scale(glm::mat4(1), _scale);

	if (_parent != nullptr)
		_localToWorld = _parent->_localToWorld * _localToWorld;
}

void			Transform::SetParent(Transform * tmp)
{
	this->_parent = tmp;

	tmp->_childs.push_back(this);

	UpdateLocalToWorldMatrix();
}

glm::vec3		Transform::GetPosition(void) const { return (this->_position); }
void			Transform::SetPosition(glm::vec3 tmp) { this->_position = tmp; UpdateLocalToWorldMatrix(); }

glm::quat		Transform::GetRotation(void) const { return (this->_rotation); }
void			Transform::SetRotation(glm::quat tmp) { this->_rotation = tmp; UpdateLocalToWorldMatrix(); }

glm::vec3		Transform::GetScale(void) const { return (this->_scale); }
void			Transform::SetScale(glm::vec3 tmp) { this->_scale = tmp; UpdateLocalToWorldMatrix(); }

glm::vec4		Transform::GetParentRight(void) const noexcept
{
	if (_parent != nullptr)
		return glm::vec4(_parent->GetRight(), 0);
	return glm::vec4(1, 0, 0, 0);
}

glm::vec4		Transform::GetParentUp(void) const noexcept
{
	if (_parent != nullptr)
		return glm::vec4(_parent->GetUp(), 0);
	return glm::vec4(0, 1, 0, 0);
}

glm::vec4		Transform::GetParentForward(void) const noexcept
{
	if (_parent != nullptr)
		return glm::vec4(_parent->GetForward(), 0);
	return glm::vec4(0, 0, 1, 0);
}

// homogenous coords for directions
glm::vec3		Transform::GetUp(void) const { return _rotation * GetParentUp(); }
glm::vec3		Transform::GetDown(void) const { return _rotation * -GetParentUp(); }
glm::vec3		Transform::GetRight(void) const { return _rotation * GetParentRight(); }
glm::vec3		Transform::GetLeft(void) const { return _rotation * -GetParentRight(); }
glm::vec3		Transform::GetForward(void) const { return _rotation * GetParentForward(); }
glm::vec3		Transform::GetBack(void) const { return _rotation * -GetParentForward(); }

glm::vec3		Transform::GetEulerAngles(void) const { return glm::eulerAngles(_rotation) * Math::DegToRad; }
glm::mat4x4		Transform::GetLocalToWorldMatrix(void) const { return _localToWorld; }

GameObject *	Transform::GetGameObject(void) { return _gameObject; }
Transform *		Transform::GetParent(void) const { return _parent; }

std::ostream &	operator<<(std::ostream & o, Transform const & r)
{
	o << "Pos: " << r.GetPosition() << ", Rot: " << r.GetRotation() << ", Scale: " << r.GetScale() << std::endl;
	(void)r;
	return (o);
}
