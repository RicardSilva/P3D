#pragma once
#include "vec.h"

struct light {
	vec3 position;
	vec3 color;

	light(const vec3 &position, const vec3 &color) : position(position), color(color) {}

	vec3 ComputeL(const vec3 &hitpoint) {
		vec3 l = position - hitpoint;
		l.Normalize();
		return l;
	}
};