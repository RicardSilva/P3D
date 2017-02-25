#pragma once

#include "vec.h"
#include "mat.h"

class Camera {
protected:

	mat4 viewMatrix;
	mat4 projMatrix;


public:
	Camera();
	Camera(vec4 &v);

	virtual ~Camera();

	void setViewMatrix(mat4 &m);
	mat4 getViewMatrix();

	void setProjMatrix(mat4 &m);
	mat4 getProjMatrix();

};

