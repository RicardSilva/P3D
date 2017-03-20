#pragma once
#include "vec.h"
#include "material.h"
#include "object.h"
#include "ray.h"

class Sphere : public Object {
	vec3 centre;
	float radius;

public:
	Sphere() {}
	Sphere(const vec3 &centre, float radius, const material &mat) : Object(mat), centre(centre), radius(radius) {}


	float GetEnterRefractionIndex(const Ray &Ray);
	float GetExitRefractionIndex(const Ray &Ray);
	bool CheckRayCollision(const Ray &ray, float *distance, vec3 *hitpoint);
	vec3 GetNormal(const Ray &ray, const vec3 &point);
};