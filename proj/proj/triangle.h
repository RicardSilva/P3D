#pragma once
#include "Object.h"
#include "material.h"
#include "vec.h"
#include <vector>

class Triangle : public Object {

	vec3 point1;
	vec3 point2;
	vec3 point3;
	
	vec3 normal;
public:
	Triangle(std::vector<vec3> points, const material &mat) 
		: Object(mat), point1(points[0]), point2(points[1]), point3(points[2]) {
		vec3 v1, v2;

		v1 = point2 - point1;
		v2 = point3 - point1;

		normal = CrossProduct(v1, v2);
		normal.Normalize();

		InitializeBoundingBox(points);
		
	}

	virtual bool CheckRayCollision(const Ray &Ray, float *distance, vec3 *hitpoint);
	virtual vec3 GetNormal(const Ray &Ray, const vec3 &point);
private:
	void InitializeBoundingBox(std::vector<vec3> points);
};