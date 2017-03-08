#pragma once
#include "vec.h"
#include "object.h"
#include "ray.h"

struct plane : public object {

	vec3 point1;
	vec3 point2;
	vec3 point3;

	plane() {}
	plane(vec3 &point1, vec3 &point2, vec3 &point3, material &mat) : object(mat), point1(point1), point2(point2), point3(point3)  {}

public:
	virtual float object::CheckRayIntersection(ray &ray);
};