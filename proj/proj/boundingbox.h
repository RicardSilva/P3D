#pragma once
#include "vec.h"
#include "ray.h"


//AXIS ALIGNED BOUNDING BOX (AABB)
struct BoundingBox {
	vec3 min;
	vec3 max;

	BoundingBox() {} //TODO: REMOVE
	BoundingBox(const vec3 &p0, const vec3 &p1) : min(p0), max(p1) {}

	float GetLength() { return max.x - min.x; }
	float GetHeight() { return max.y - min.y; }
	float GetWidth() { return max.z - min.z; }

	bool isInside(const vec3 &point);
	bool CheckRayCollision(const Ray &ray, float *tnear, float *tfar, vec3 *hitnear, vec3 *hitfar);

};