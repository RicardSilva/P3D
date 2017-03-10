#pragma once
#include "material.h"
#include "vec.h"
#include "ray.h"

class object {

	material mat;

public:
	object() {}
	object(const material &mat) : mat(mat) {}

	const material GetMaterial() {
		return mat;
	}

	//RETURNS FIRST HIT POINT IF IT EXISTS
	virtual bool CheckRayCollision(const ray &ray, float *distance, vec3 *hitpoint) = 0;
	virtual vec3 ComputeNormal(const ray &ray, const vec3 &point) = 0;

};