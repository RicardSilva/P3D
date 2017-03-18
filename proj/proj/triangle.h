#pragma once
#include "Object.h"
#include "material.h"
#include "vec.h"
#include <vector>

class Triangle : public Object {

	vec3 point1;
	vec3 point2;
	vec3 point3;

	vec3 v1;
	vec3 v2;
	
	vec3 normal;
public:
	Triangle() {}
	Triangle(std::vector<vec3> points, const material &mat) 
		: Object(mat), point1(points[0]), point2(points[1]), point3(points[2]) {
	
		this->v1 = point2 - point1;
		this->v2 = point3 - point1;

		this->normal = CrossProduct(v1, v2);
		this->normal.Clean();
		this->normal.Normalize();
	}

	virtual bool CheckRayCollision(const Ray &Ray, float *distance, vec3 *hitpoint);
	virtual vec3 GetNormal(const Ray &Ray, const vec3 &point);
};