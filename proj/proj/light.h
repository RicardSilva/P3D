#pragma once
#include "vec.h"

struct light {
	vec3 position;
	vec3 color;

	light(vec3 &position, vec3 &color) : position(position), color(color) {}
};