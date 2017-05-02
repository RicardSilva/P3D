#pragma once
#include "triangle.h"
#include "material.h"
#include "vec.h"
#include <vector>

class TriangleInv : public Triangle {

public:
	TriangleInv(std::vector<vec3> points, const material &mat)
		: Triangle(points, mat) {

		normal = -1 * normal;

	}
};