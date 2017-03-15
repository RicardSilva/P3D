#pragma once
#include "object.h"
#include "material.h"
#include "vec.h"
#include <vector>

class triangle : public object {

	vec3 point1;
	vec3 point2;
	vec3 point3;
	
	vec3 normal;
public:
	triangle() {}
	triangle(std::vector<vec3> points, const material &mat) 
		: object(mat), point1(points[0]), point2(points[1]), point3(points[2]) {
		vec3 v1, v2;

		v1 = point2 - point1;
		v2 = point3 - point1;

		normal = CrossProduct(v1, v2);
		normal.Normalize();
	}

	virtual bool CheckRayCollision(const ray &ray, float *distance, vec3 *hitpoint);
	virtual vec3 GetNormal(const ray &ray, const vec3 &point);
};