#pragma once
#include "vec.h"
#include "object.h"
#include "ray.h"

class plane : public object {

	vec3 normal;
	float d;

public:
	plane() {}
	plane(const vec3 &point1, const vec3 &point2, const vec3 &point3, const material &mat) 
		: object(mat) {

		vec3 n;
		float z;
		vec3 v1, v2;

		v1 = point1 - point2;
		v2 = point3 - point2;

		n = CrossProduct(v1, v2);
		z = DotProduct(n, point1);

		float magnitude = n.Magnitude();

		normal = n / magnitude;
		d = z / magnitude;

	}

	virtual bool object::CheckRayCollision(const ray &ray, float *distance, vec3 *hitpoint);
	virtual vec3 ComputeNormal(const ray &ray, const vec3 &point);
};