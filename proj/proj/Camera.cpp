#include "Camera.h"

namespace engine {

	Camera::Camera() {}

	Camera::Camera(vec4 &v) : clippingPlane(v){}

	Camera::~Camera() {
	}

	
	void Camera::setViewMatrix(mat4 &m) { viewMatrix = m; }
	mat4 Camera::getViewMatrix() { return viewMatrix; }

	void Camera::setProjMatrix(mat4 &m) { projMatrix = m; }
	mat4 Camera::getProjMatrix() { return projMatrix; }

	
}