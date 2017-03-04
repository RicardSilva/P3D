#pragma once

#include "vec.h"
#include "mat.h"

struct camera {
	
	vec3 from;
	vec3 at;
	vec3 up;
	int angle;
	float hither;
	int resolutionX;
	int resolutionY;


public:
	camera() {}

};

