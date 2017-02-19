#pragma once

#include "vec.h"
#include "mat.h"

//using namespace std; 

namespace engine {

	class Camera {
	protected:

		vec4 clippingPlane;

		mat4 viewMatrix;
		mat4 projMatrix;


	public:
		Camera();
		Camera(vec4 &v);

		virtual ~Camera();

		void setClippingPlane(vec4& v) { clippingPlane = v; }
		vec4 getClippingPlane() { return clippingPlane; }

		void setViewMatrix(mat4 &m);
		mat4 getViewMatrix();

		void setProjMatrix(mat4 &m);
		mat4 getProjMatrix();

	};

}