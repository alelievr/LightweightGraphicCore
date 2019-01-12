/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Transform.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/31 11:42:19 by alelievr          #+#    #+#             */
/*   Updated: 2018/11/05 19:17:29 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "IncludeDeps.hpp"

#include GLM_INCLUDE_QUATERNION
#include GLM_INCLUDE_MATRIX_TRANSFORM

#include <memory>
#include <iostream>
#include <string>
#include <vector>

namespace LWGC
{
	class		Transform
	{
		private:
			std::shared_ptr< Transform >	_parent;
			std::vector< Transform * >		_childs;
			glm::vec3						_position;
			glm::quat						_rotation;
			glm::vec3						_scale;
			float							_yaw;
			float							_pitch;
			float							_roll;

			glm::mat4x4						_localToWorld;
			glm::vec3						_up;
			glm::vec3						_right;
			glm::vec3						_forward;

			void		UpdatePositionDatas(void) noexcept;
			void		UpdateRotationDatas(void) noexcept;
			void		UpdateScaleDatas(void) noexcept;
			void		UpdateLocalToWorldMatrix(void) noexcept;
			glm::vec4	GetParentUp(void) const noexcept;
			glm::vec4	GetParentRight(void) const noexcept;
			glm::vec4	GetParentForward(void) const noexcept;

		public:
			Transform(void);
			Transform(const Transform &) = delete;
			virtual ~Transform(void);

			Transform &	operator=(Transform const & src) = delete;

			void		Rotate(const glm::vec3 & angleInRadians);
			void		Rotate(const float xAngle, const float yAngle, const float zAngle);
			void		RotateAxis(float angle, const glm::vec3 & axis);
			void		RotateAround(const glm::vec3 & point, const glm::vec3 & axis, const float angle);
			size_t		GetChildCount(void);
			bool		IsChildOf(Transform * t);
			Transform *	GetChildAt(const int index) const;
			void		LookAt(const glm::vec3 & direction, const glm::vec3 & up = glm::vec3(0, 1, 0));
			glm::vec3	TransformDirection(const glm::vec3 & direction);
			glm::vec3	TransformDirection(const float x, const float y, const float z);
			glm::vec3	TransformPoint(const glm::vec3 & position);
			glm::vec3	TransformPoint(const float x, const float y, const float z);
			void		Translate(const glm::vec3 & translation);
			void		Scale(const glm::vec3 & scaleFactor);

			Transform *	GetRoot(void);
			std::shared_ptr< Transform >	GetParent(void) const;
			void		AddChild(std::shared_ptr< Transform > child);
			void		RemoveChild(std::shared_ptr< Transform > child);
			void		SetParent(std::shared_ptr< Transform > tmp);
			glm::vec3	GetPosition(void) const;
			void		SetPosition(glm::vec3 tmp);
			glm::quat	GetRotation(void) const;
			void		SetRotation(glm::quat tmp);
			glm::vec3	GetScale(void) const;
			void		SetScale(glm::vec3 tmp);

			glm::vec3	GetUp(void) const;
			glm::vec3	GetDown(void) const;
			glm::vec3	GetRight(void) const;
			glm::vec3	GetLeft(void) const;
			glm::vec3	GetForward(void) const;
			glm::vec3	GetBack(void) const;
			glm::vec3	GetEulerAngles(void) const;
			glm::mat4x4	GetLocalToWorldMatrix(void) const;
	};

	std::ostream &	operator<<(std::ostream & o, Transform const & r);
}
