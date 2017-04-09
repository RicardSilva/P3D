#pragma once
#include "vec.h"

//AXIS ALIGNED BOUNDING BOX (AABB)
struct BoundingBox {
	vec3 min;
	vec3 max;

	BoundingBox() {} //TODO: REMOVE
	BoundingBox(const vec3 &p0, const vec3 &p1) : min(p0), max(p1) {}

};