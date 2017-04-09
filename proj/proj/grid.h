#pragma once
#include "object.h"
#include "boundingbox.h"
#include "vec.h"
#include <vector>

#define EPSILON 0.0001

struct Grid {

	BoundingBox bb;
	float wx, wy, wz;
	int objCount;
	float m = 2.0f; // factor that allows to vary the number of cells

	Grid(std::vector<Object*> objects) {
		objCount = objects.size();
		InitializeBoundingBox(objects);
	}

private:
	void InitializeBoundingBox(std::vector<Object*> objects) {
		vec3 p0 = vec3(FLT_MAX, FLT_MAX, FLT_MAX);
		vec3 p1 = vec3(FLT_MIN, FLT_MIN, FLT_MIN);

		for (auto &obj : objects) {
			BoundingBox obb = obj->GetBoundingBox();
			if (obb.min.x < p0.x)
				p0.x = obb.min.x;
			if (obb.min.y < p0.y)
				p0.y = obb.min.y;
			if (obb.min.z < p0.z)
				p0.z = obb.min.z;
			if (obb.max.x > p1.x)
				p1.x = obb.max.x;
			if (obb.max.x > p1.x)
				p1.x = obb.max.x;
			if (obb.max.x > p1.x)
				p1.x = obb.max.x;
		}
		p0.x -= EPSILON; p0.y -= EPSILON; p0.z -= EPSILON;
		p1.x += EPSILON; p1.y += EPSILON; p1.z += EPSILON;

		bb = BoundingBox(p0, p1);
	
		wx = p1.x - p0.x; 
		wy = p1.y - p0.y; 
		wz = p1.z - p0.z;
	}
	void InitializeCells() {}
};