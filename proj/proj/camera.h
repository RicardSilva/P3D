#pragma once

#include "vec.h"
#include "mat.h"
#include "ray.h"

struct camera {
	
	vec3 eye;
	vec3 at;
	vec3 up;
	int fovy;
	float nearp, farp;

	int resolutionX;
	int resolutionY;

	float w, h;
	vec3 xe, ye, ze;
	float df;


public:
	camera() {}
	camera(vec3 eye, vec3 at, vec3 up, float fovy, float near, float far, int resX, int resY) 
		: eye(eye), at(at), up(up), fovy(fovy), nearp(near), farp(far), resolutionX(resX), resolutionY(resY)
	{
		df = (eye - at).Magnitude();

		h = 2 * df * tan(fovy / 2);
		w = resolutionX / resolutionY * h;
		
		ze = (1 / df) * (eye - at);
		vec3 cross = CrossProduct(up, ze);
		xe = (1 / cross.Magnitude()) * cross;
		ye = CrossProduct(ze, xe);

	}

	Ray getPrimaryRay(int x, int y);

};

