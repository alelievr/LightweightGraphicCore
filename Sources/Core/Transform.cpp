#include "Transform.hpp"

#include <algorithm>
#include "Utils/Math.hpp"
#include "Utils/Vector.hpp"

using namespace LWGC;

Transform::Transform(void)
{
	this->_parent = nullptr;
	this->_position = glm::vec3(0, 0, 0);
	this->_rotation = glm::quat(1, 0, 0, 0);
	this->_scale = glm::vec3(1, 1, 1);
	// TODO: replace pitch, yaw and roll by quaternions
	this->_yaw = 0;
	this->_pitch = 0;
	this->_roll = 0;
}

Transform::~Transform(void)
{
}

void		Transform::Rotate(const glm::vec3 & angleInradians)
{
	_yaw += angleInradians.x;
	_pitch += angleInradians.y;
	_roll += angleInradians.z;

	//FPS camera:  RotationX(pitch) * RotationY(yaw)
	glm::quat qPitch = glm::angleAxis(_pitch, glm::vec3(1, 0, 0));
	glm::quat qYaw = glm::angleAxis(_yaw, glm::vec3(0, 1, 0));

	// std::cout << "_yaw: " << _yaw << ", pitch: " << _pitch << ", roll: " << _roll << "\n";

	//For a FPS camera we can omit roll
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

bool		Transform::IsChildOf(std::shared_ptr< Transform > t)
{
	return std::find(t->_childs.begin(), t->_childs.end(), t) != t->_childs.end();
}

std::shared_ptr< Transform >		Transform::GetChildAt(const int index) const
{
	return _childs[index];
}

void		Transform::LookAt(const int index)
{
	// TODO: change the lookat parameter, it make no sence right now
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
	_localToWorld = glm::translate(glm::mat4(1.0f), _position) * glm::toMat4(_rotation) * glm::scale(glm::mat4(1), _scale);
}

void			Transform::SetParent(std::shared_ptr< Transform > tmp) { this->_parent = tmp; }

glm::vec3		Transform::GetPosition(void) const { return (this->_position); }
void			Transform::SetPosition(glm::vec3 tmp) { this->_position = tmp; UpdateLocalToWorldMatrix(); }

glm::quat		Transform::GetRotation(void) const { return (this->_rotation); }
void			Transform::SetRotation(glm::quat tmp) { this->_rotation = tmp; UpdateLocalToWorldMatrix(); }

glm::vec3		Transform::GetScale(void) const { return (this->_scale); }
void			Transform::SetScale(glm::vec3 tmp) { this->_scale = tmp; UpdateLocalToWorldMatrix(); }

// homogenous coords for directions
glm::vec3		Transform::GetUp(void) const { return _rotation *  glm::vec4(0, 1, 0, 0); }
glm::vec3		Transform::GetDown(void) const { return  _rotation * glm::vec4(0, -1, 0, 0); }
glm::vec3		Transform::GetRight(void) const { return _rotation *  glm::vec4(1, 0, 0, 0); }
glm::vec3		Transform::GetLeft(void) const { return  _rotation * glm::vec4(-1, 0, 0, 0); }
glm::vec3		Transform::GetForward(void) const { return _rotation *  glm::vec4(0, 0, 1, 0);; }
glm::vec3		Transform::GetBack(void) const { return  _rotation * glm::vec4(0, 0, -1, 0); }

glm::vec3		Transform::GetEulerAngles(void) const { return glm::eulerAngles(_rotation) * Math::DegToRad; }
glm::mat4x4		Transform::GetLocalToWorldMatrix(void) const { return _localToWorld; }

std::ostream &	operator<<(std::ostream & o, Transform const & r)
{
	o << "Pos: " << r.GetPosition() << ", Rot: " << r.GetRotation() << ", Scale: " << r.GetScale() << std::endl;
	(void)r;
	return (o);
}
