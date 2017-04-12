#pragma once
#include "object.h"
#include "boundingbox.h"
#include "vec.h"
#include "ray.h"
#include <vector>
#include <array>
#include <algorithm>

#define EPSILON 0.00001

struct Grid {

	BoundingBox bbox;
	std::vector<std::vector<Object*>> cells;
	int nx, ny, nz;
	int objCount;
	float m = 2.0f; // factor that allows to vary the number of cells

	Grid() {}
	Grid(std::vector<Object*> objects) {
		objCount = objects.size();
		InitializeBoundingBox(objects);
		InitializeCells(objects);
	}
	~Grid() {}
	
	bool Traverse(const Ray &ray, Object **hitobject, vec3 *hitpoint);
	bool Traverse(const Ray &ray);

private:
	void InitializeBoundingBox(std::vector<Object*> objects);
	void InitializeCells(std::vector<Object*> objects);

	inline float clamp(float x, float a, float b) {
		return x < a ? a : (x > b ? b : x);
	}


};