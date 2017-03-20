#pragma once

#include "vec.h"

struct material {
	vec3 color;
	float Kd;
	float Ks;
	float shine;
	float t;
	float refraction_index;

	material() {}
	material(const vec3 &color, float Kd, float Ks, float shine, float t, float refraction_index)
		: color(color), Kd(Kd), Ks(Ks), shine(shine), t(t), refraction_index(refraction_index) {}
};
