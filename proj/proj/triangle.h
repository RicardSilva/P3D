#pragma once
#include "Object.h"
#include "material.h"
#include "vec.h"
#include <vector>

class Triangle : public Object {

	vec3 vertice1;
	vec3 vertice2;
	vec3 vertice3;

	vec3 v1;
	vec3 v2;
	
	vec3 normal;


public:
	Triangle() {}
	Triangle(std::vector<vec3> points, const material &mat) 
		: Object(mat), vertice1(points[0]), vertice2(points[1]), vertice3(points[2]) {
	
		this->v1 = vertice2 - vertice1;
		this->v2 = vertice3 - vertice1;

		this->normal = CrossProduct(v1, v2);
		this->normal.Clean();
		this->normal.Normalize();
	}


	bool CheckRayCollision(const Ray &Ray, float *distance, vec3 *hitpoint);
	vec3 GetNormal(const Ray &Ray, const vec3 &point);
};