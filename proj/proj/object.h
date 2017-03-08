#pragma once
#include "material.h"
#include "vec.h"
#include "ray.h"

struct object {
	material mat;

	object() {}
	object(material &mat) : mat(mat) {}


	//RETURNS FIRST HIT POINT IF IT EXISTS
	virtual float CheckRayIntersection(ray &ray) = 0;

};