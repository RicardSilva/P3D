#include "grid.h"

bool Grid::Traverse(const Ray &ray) {

	float tnear, tfar;
	vec3 hitnear, hitfar;

	if (bb.CheckRayCollision(ray, &tnear, &tfar, &hitfar, &hitfar) == false)
		return false;

	int x, y, z;
	vec3 origin = ray.origin;
	vec3 direction = ray.direction;
	vec3 min = bb.min;
	vec3 max = bb.max;
	if (bb.isInside(origin)) {
		x = clamp((origin.x - min.x) * nx / (max.x - min.x), 0, nx - 1);
		y = clamp((origin.y - min.y) * ny / (max.y - min.y), 0, ny - 1);
		z = clamp((origin.z - min.z) * nz / (max.z - min.z), 0, nz - 1);
	}
	else {
		x = clamp((hitnear.x - min.x) * nx / (max.x - min.x), 0, nx - 1);
		y = clamp((hitnear.y - min.y) * ny / (max.y - min.y), 0, ny - 1);
		z = clamp((hitnear.z - min.z) * nz / (max.z - min.z), 0, nz - 1);
	}

	float dtx = (tfar - tnear) / nx;
	float dty = (tfar - tnear) / ny;
	float dtz = (tfar - tnear) / nz;

	float nextx, nexty, nextz;
	int stepx, stepy, stepz;
	int stopx, stopy, stopz;
}


void Grid::InitializeBoundingBox(std::vector<Object*> objects) {
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
}
void Grid::InitializeCells(std::vector<Object*> objects) {
	float s = pow((length * height * width / objCount), 1 / 3);
	nx = trunc(m * length / s) + 1;
	ny = trunc(m * height / s) + 1;
	nz = trunc(m * width / s) + 1;

	float cellCount = nx * ny * nz;

	cells = new std::vector<Object*>[cellCount];
	int xmin, xmax;
	int ymin, ymax;
	int zmin, zmax;
	for (auto &obj : objects) {
		/* Compute indices of both cells that contain min and max coord of obj bbox */
		BoundingBox obb = obj->GetBoundingBox();

		xmin = clamp((obb.min.x - bb.min.x) * nx / (bb.max.x - bb.min.x), 0, nx - 1);
		ymin = clamp((obb.min.y - bb.min.y) * ny / (bb.max.y - bb.min.y), 0, ny - 1);
		zmin = clamp((obb.min.z - bb.min.z) * nz / (bb.max.z - bb.min.z), 0, nz - 1);
		xmax = clamp((obb.max.x - bb.min.x) * nx / (bb.max.x - bb.min.x), 0, nx - 1);
		ymax = clamp((obb.max.y - bb.min.y) * ny / (bb.max.y - bb.min.y), 0, ny - 1);
		zmax = clamp((obb.max.z - bb.min.z) * nz / (bb.max.z - bb.min.z), 0, nz - 1);		for (int z = zmin; z <= zmax; z++) {			for (int y = ymin; y <= ymax; y++) {				for (int x = xmin; x <= xmax; x++) {					int index = x + nx * y + nx * ny * z;					cells[index].push_back(obj);				}			}		}
	}

}