#pragma once
#include "vec.h"
#include "object.h"

struct plane : object {

	vec3 point1;
	vec3 point2;
	vec3 point3;

	plane() : point1(), point2(), point3() {}

};