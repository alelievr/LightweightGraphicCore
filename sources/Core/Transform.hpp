/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Transform.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/31 11:42:19 by alelievr          #+#    #+#             */
/*   Updated: 2017/08/31 11:43:42 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TRANSFORM_HPP
# define TRANSFORM_HPP
# include <iostream>
# include <string>
# include <list>

class		Transform
{
	private:
		Transform *	_parent;
		std::list< Transform >	_childs;
		glm::vec3	_position;
		glm::quat	_rotation;
		glm::vec3	_scale;
		glm::vec3	_up;
		glm::vec3	_down;
		glm::vec3	_right;
		glm::vec3	_left;
		glm::vec3	_forward;
		glm::vec3	_back;
		glm::vec3	_eulerAngles;


	public:
		Transform(void);
		Transform(const Transform&);
		virtual ~Transform(void);

		Transform &	operator=(Transform const & src);

		void	Rotate(const glm::vec3 & eulerAngles);

		void	Rotate(const float xAngle, const float yAngle, const float zAngle);

		void	RotateAround(const glm::vec3 & point, const glm::vec3 & axis, const float angle);

		size_t	GetChildCount(void);

		bool	IsChildOf(const Transform & t);

		Transform	GetChildAt(const int index) const;

		void	LookAt(const int index);

		glm::vec3	TransformDirection(const glm::vec3 & direction);

		glm::vec3	TransformDirection(const float x, const float y, const float z);

		glm::vec3	TransformPoint(const glm::vec3 & position);

		glm::vec3	TransformPoint(const float x, const float y, const float z);

		glm::vec3	TransformDirection(const glm::vec3 & direction);

		void	Translate(const glm::vec3 & translation);

		Transform &	GetRoot(void);

		Transform *	getParent(void) const;
		void	setParent(Transform * tmp);
		
		std::list< Transform >	getChilds(void) const;
		void	setChilds(std::list< Transform > tmp);
		
		glm::vec3	getPosition(void) const;
		void	setPosition(glm::vec3 tmp);
		
		glm::quat	getRotation(void) const;
		void	setRotation(glm::quat tmp);
		
		glm::vec3	getScale(void) const;
		void	setScale(glm::vec3 tmp);
		
		glm::vec3	getUp(void) const;
		void	setUp(glm::vec3 tmp);
		
		glm::vec3	getDown(void) const;
		void	setDown(glm::vec3 tmp);
		
		glm::vec3	getRight(void) const;
		void	setRight(glm:vec3 tmp);
		
		glm::vec3	getLeft(void) const;
		void	setLeft(glm::vec3 tmp);
		
		glm::vec3	getForward(void) const;
		void	setForward(glm::vec3 tmp);
		
		glm::vec3	getBack(void) const;
		void	setBack(glm::vec3 tmp);
		
		glm::vec3	getEulerAngles(void) const;
		void	setEulerAngles(glm::vec3 tmp);
};

std::ostream &	operator<<(std::ostream & o, Transform const & r);

#endif
