#pragma once
#include "material.h"
#include "vec.h"

struct object {
	vec3 pos;
	material mat;

	object() : pos(), mat() {}
	object(vec3 pos, material mat) : pos(pos), mat(mat) {}

};