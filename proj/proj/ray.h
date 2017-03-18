#pragma once
#include "vec.h"

struct Ray {

	vec3 origin;
	vec3 direction;

	Ray() : origin(vec3()), direction(vec3()) {}
	Ray(vec3 &origin, vec3 &direction) : origin(origin), direction(direction) {}
};