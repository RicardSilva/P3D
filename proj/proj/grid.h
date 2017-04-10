#pragma once
#include "object.h"
#include "boundingbox.h"
#include "vec.h"
#include "ray.h"
#include <vector>
#include <array>
#include <algorithm>

#define EPSILON 0.0001

struct Grid {

	BoundingBox bb;
	std::vector<Object*>* cells;
	float length, height, width;
	int nx, ny, nz;
	int objCount;
	float m = 2.0f; // factor that allows to vary the number of cells

	Grid(std::vector<Object*> objects) {
		objCount = objects.size();
		InitializeBoundingBox(objects);

		length = bb.GetLength();
		height = bb.GetHeight();
		width = bb.GetWidth();

		InitializeCells(objects);
	}
	~Grid() {
		delete [] cells;
	}

	
	bool Traverse(const Ray &ray);

private:
	void InitializeBoundingBox(std::vector<Object*> objects);
	void InitializeCells(std::vector<Object*> objects);

	inline float clamp(float x, float a, float b) {
		return x < a ? a : (x > b ? b : x);
	}


};