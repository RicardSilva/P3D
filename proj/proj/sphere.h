#pragma once
#include "vec.h"
#include "material.h"
#include "object.h"
#include "ray.h"

struct sphere : public object {
	vec3 position;
	float radius;

	sphere() {}
	sphere(vec3 &pos, float radius, material &mat) : object(mat), position(pos), radius(radius) {}

public:
	virtual float CheckRayIntersection(ray &ray);
};