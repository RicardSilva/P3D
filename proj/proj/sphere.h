#pragma once
#include "vec.h"
#include "material.h"
#include "object.h"

struct sphere : object {
	float radius;

	sphere() {}
	sphere(vec3 pos, float radius, material mat) : object(pos, mat), radius(radius) {}
};