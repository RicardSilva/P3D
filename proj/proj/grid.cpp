#include "grid.h"
/*
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

	//ray direction (dx, dy, dz)
	//dx
	if (direction.x > 0) {
		nextx = tnear + (x + 1) * dtx;
		stepx = 1;
		stopx = nx;
	}
	else {
		nextx = tnear + (nx - x) * dtx;
		stepx = -1;
		stopx = -1;
	}
	if (direction.x == 0) {
		nextx = FLT_MAX;
		stepx = -1;
		stopx = -1;
	}
	//dy
	if (direction.y > 0) {
		nexty = tnear + (y + 1) * dty;
		stepy = 1;
		stopy = ny;
	}
	else {
		nexty = tnear + (ny - y) * dty;
		stepy = -1;
		stopy = -1;
	}
	if (direction.y == 0) {
		nexty = FLT_MAX;
		stepy = -1;
		stopy = -1;
	}
	//dz
	if (direction.z > 0) {
		nextz = tnear + (z + 1) * dtz;
		stepz = 1;
		stopz = nz;
	}
	else {
		nextz = tnear + (nz - z) * dtz;
		stepz = -1;
		stopz = -1;
	}
	if (direction.z == 0) {
		nextz = FLT_MAX;
		stepz = -1;
		stopz = -1;
	}
	//loop
	std::vector<Object*> objs;
	while (true) {
		
	}
}
*/

bool Grid::Traverse(const Ray &ray, Object *&hitobject, vec3 *hitpoint) {
	double ox = ray.origin.x;
	double oy = ray.origin.y;
	double oz = ray.origin.z;
	double dx = ray.direction.x;
	double dy = ray.direction.y;
	double dz = ray.direction.z;

	double x0 = bbox.min.x;
	double y0 = bbox.min.y;
	double z0 = bbox.min.z;
	double x1 = bbox.max.x;
	double y1 = bbox.max.y;
	double z1 = bbox.max.z;

	double tx_min, ty_min, tz_min;
	double tx_max, ty_max, tz_max;

	double a = 1.0 / dx;
	if (a >= 0) {
		tx_min = (x0 - ox) * a;
		tx_max = (x1 - ox) * a;
	}
	else {
		tx_min = (x1 - ox) * a;
		tx_max = (x0 - ox) * a;
	}

	double b = 1.0 / dy;
	if (b >= 0) {
		ty_min = (y0 - oy) * b;
		ty_max = (y1 - oy) * b;
	}
	else {
		ty_min = (y1 - oy) * b;
		ty_max = (y0 - oy) * b;
	}

	double c = 1.0 / dz;
	if (c >= 0) {
		tz_min = (z0 - oz) * c;
		tz_max = (z1 - oz) * c;
	}
	else {
		tz_min = (z1 - oz) * c;
		tz_max = (z0 - oz) * c;
	}

	double t0, t1;

	if (tx_min > ty_min)
		t0 = tx_min;
	else
		t0 = ty_min;

	if (tz_min > t0)
		t0 = tz_min;

	if (tx_max < ty_max)
		t1 = tx_max;
	else
		t1 = ty_max;

	if (tz_max < t1)
		t1 = tz_max;

	if (t0 > t1)
		return(false);


	// initial cell coordinates

	int ix, iy, iz;

	if (bbox.isInside(ray.origin)) {  			// does the ray start inside the grid?
		ix = clamp((ox - x0) * nx / (x1 - x0), 0, nx - 1);
		iy = clamp((oy - y0) * ny / (y1 - y0), 0, ny - 1);
		iz = clamp((oz - z0) * nz / (z1 - z0), 0, nz - 1);
	}
	else {
		vec3 p = ray.origin + t0 * ray.direction;  // initial hit point with grid's bounding box
		ix = clamp((p.x - x0) * nx / (x1 - x0), 0, nx - 1);
		iy = clamp((p.y - y0) * ny / (y1 - y0), 0, ny - 1);
		iz = clamp((p.z - z0) * nz / (z1 - z0), 0, nz - 1);
	}

	// ray parameter increments per cell in the x, y, and z directions

	double dtx = (tx_max - tx_min) / nx;
	double dty = (ty_max - ty_min) / ny;
	double dtz = (tz_max - tz_min) / nz;

	double 	tx_next, ty_next, tz_next;
	int 	ix_step, iy_step, iz_step;
	int 	ix_stop, iy_stop, iz_stop;

	if (dx > 0) {
		tx_next = tx_min + (ix + 1) * dtx;
		ix_step = +1;
		ix_stop = nx;
	}
	else {
		tx_next = tx_min + (nx - ix) * dtx;
		ix_step = -1;
		ix_stop = -1;
	}

	if (dx == 0.0) {
		tx_next = FLT_MAX;
		ix_step = -1;
		ix_stop = -1;
	}


	if (dy > 0) {
		ty_next = ty_min + (iy + 1) * dty;
		iy_step = +1;
		iy_stop = ny;
	}
	else {
		ty_next = ty_min + (ny - iy) * dty;
		iy_step = -1;
		iy_stop = -1;
	}

	if (dy == 0.0) {
		ty_next = FLT_MAX;
		iy_step = -1;
		iy_stop = -1;
	}

	if (dz > 0) {
		tz_next = tz_min + (iz + 1) * dtz;
		iz_step = +1;
		iz_stop = nz;
	}
	else {
		tz_next = tz_min + (nz - iz) * dtz;
		iz_step = -1;
		iz_stop = -1;
	}

	if (dz == 0.0) {
		tz_next = FLT_MAX;
		iz_step = -1;
		iz_stop = -1;
	}


	// traverse the grid
	std::vector<Object*> objs;
	float t_max;
	while (true) {
		if (tx_max < ty_max) {
			if (tx_max < tz_max) {
				ix = ix + ix_step;
				if (ix == ix_stop)
					return false;
				t_max = tx_max;
				tx_max = tx_max + dtx;
			}
			else {
				iz = iz + iz_step;
				if (iz == iz_stop)
					return false;
				t_max = tz_max;
				tz_max = tz_max + dtz;
			}
		}
		else {
			if (ty_max < tz_max) {
				iy = iy + iy_step;
				if (iy == iy_stop)
					return false;
				t_max = ty_max;
				ty_max = ty_max + dty;
			}
			else {
				iz = iz + iz_step;
				if (iz == iz_stop)
					return false;
				t_max = tz_max;
				tz_max = tz_max + dtz;
			}
		}
		int index = ix + nx * iy + nx * ny * iz;
		objs = cells[index];
		if (objs.size() != 0) {
			bool hit = false;
			float closestDistance = FLT_MAX;
			vec3 closestHitpoint;
			Object* closestObj = nullptr;
			float distance;
			vec3 hitpnt;


			//intersect Ray with all objects and find a hit point(if any) closest to the start of the Ray
			for (auto &obj : objs) {
				if (obj->CheckRayCollision(ray, &distance, &hitpnt) == true) {

					if (distance < closestDistance) {
						hit = true;
						closestDistance = distance;
						closestObj = obj;
						closestHitpoint = hitpnt;
					}
				}
			}
			if (hit == true && distance <= t_max) {
				
				hitobject = closestObj;
				if (hitpoint != nullptr)
					hitpoint = &closestHitpoint;
				return true;
			}
		}
	}

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
		if (obb.max.y > p1.y)
			p1.y = obb.max.y;
		if (obb.max.z > p1.z)
			p1.z = obb.max.z;
	}
	p0.x -= EPSILON; p0.y -= EPSILON; p0.z -= EPSILON;
	p1.x += EPSILON; p1.y += EPSILON; p1.z += EPSILON;

	bbox = BoundingBox(p0, p1);
}
void Grid::InitializeCells(std::vector<Object*> objects) {
	float s = pow((length * height * width / objCount), 1 / 3);
	nx = trunc(m * length / s) + 1;
	ny = trunc(m * height / s) + 1;
	nz = trunc(m * width / s) + 1;

	float cellCount = nx * ny * nz;
	std::vector<Object*> empty;
	for (int i = 0; i < cellCount; i++)
		cells.push_back(empty);
	//cells = new std::vector<Object*>[cellCount];
	int xmin, xmax;
	int ymin, ymax;
	int zmin, zmax;
	for (auto &obj : objects) {
		/* Compute indices of both cells that contain min and max coord of obj bbox */
		BoundingBox obb = obj->GetBoundingBox();

		xmin = clamp((obb.min.x - bbox.min.x) * nx / (bbox.max.x - bbox.min.x), 0, nx - 1);
		ymin = clamp((obb.min.y - bbox.min.y) * ny / (bbox.max.y - bbox.min.y), 0, ny - 1);
		zmin = clamp((obb.min.z - bbox.min.z) * nz / (bbox.max.z - bbox.min.z), 0, nz - 1);
		xmax = clamp((obb.max.x - bbox.min.x) * nx / (bbox.max.x - bbox.min.x), 0, nx - 1);
		ymax = clamp((obb.max.y - bbox.min.y) * ny / (bbox.max.y - bbox.min.y), 0, ny - 1);
		zmax = clamp((obb.max.z - bbox.min.z) * nz / (bbox.max.z - bbox.min.z), 0, nz - 1);

		for (int z = zmin; z <= zmax; z++) {
			for (int y = ymin; y <= ymax; y++) {
				for (int x = xmin; x <= xmax; x++) {
					int index = x + nx * y + nx * ny * z;
					cells[index].push_back(obj);
				}
			}
		}

	}

}