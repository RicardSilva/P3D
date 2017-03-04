#pragma once

#include "vec.h"

struct material {
	vec3 color;
	float Kd;
	float Ks;
	float shine;
	float t;
	float refraction_index;
};
