#pragma once
#include "vec.h"

struct ray {

	vec3 origin;
	vec3 direction;

	ray() : origin(vec3()), direction(vec3()) {}
	ray(vec3 &origin, vec3 &direction) : origin(origin), direction(direction) {}
};