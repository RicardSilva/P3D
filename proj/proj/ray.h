#pragma once
#include "vec.h"

struct Ray {

	vec3 origin;
	vec3 direction;

	Ray(vec3 origin, vec3 direction) : origin(origin), direction(direction) {}
};