#ifndef CAMERA_HPP
# define CAMERA_HPP
# include <iostream>
# include <string>

class		Camera
{
	private:
		RenderTarget	_target;
		glm::vec2	_size;
		CameraType	_cameraType;
		bool	_orthographic;
		float	_fov;
		float	_nearPlane;
		float	_farPlane;


	public:
		Camera();
		Camera(const Camera&);
		virtual ~Camera(void);

		Camera &	operator=(Camera const & src);

		void	Render(void);

		glm::vec3	WorldToScreenPoint(glm::vec3 worldPosition);

		glm::vec3	ScreenToWorldPoint(glm::vec3 screenPosition);

		RenderTarget	getTarget(void) const;
		void	setTarget(RenderTarget tmp);
		
		glm::vec2	getSize(void) const;
		void	setSize(glm::vec2 tmp);
		
		CameraType	getCameraType(void) const;
		void	setCameraType(CameraType tmp);
		
		bool	getOrthographic(void) const;
		void	setOrthographic(bool tmp);
		
		float	getFov(void) const;
		void	setFov(float tmp);
		
		float	getNearPlane(void) const;
		void	setNearPlane(float tmp);
		
		float	getFarPlane(void) const;
		void	setFarPlane(float tmp);
};

std::ostream &	operator<<(std::ostream & o, Camera const & r);

#endif
