#pragma once
#include "vec.h"

struct Ray {
private:
	static int rayCount;

public:
	int id;

	vec3 origin;
	vec3 direction;

	Ray() : id(++rayCount), origin(vec3()), direction(vec3()) {}
	Ray(vec3 &origin, vec3 &direction) : id(++rayCount), origin(origin), direction(direction) {}
	bool IsNull() {
		return origin == vec3() && direction == vec3();
	}
};