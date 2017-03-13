#pragma once
#include "vec.h"
#include "material.h"
#include "object.h"
#include "ray.h"

class sphere : public object {
	vec3 centre;
	float radius;

public:
	sphere() {}
	sphere(const vec3 &centre, float radius, const material &mat) : object(mat), centre(centre), radius(radius) {}

	virtual bool CheckRayCollision(const ray &ray, float *distance, vec3 *hitpoint);
	virtual vec3 GetNormal(const ray &ray, const vec3 &point);
};