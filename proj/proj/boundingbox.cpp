#include "boundingbox.h"


bool BoundingBox::isInside(const vec3 &point) {
	float x = point.x;
	float y = point.y;
	float z = point.z;

	if (x < min.x || x > max.x) return false;
	if (y < min.y || y > max.y) return false;
	if (z < min.z || z > max.z) return false;
	
	return true;
}
bool BoundingBox::CheckRayCollision(const Ray &ray, float *tnearp, float *tfarp, vec3 *hitnear, vec3 *hitfar) {
	// Kay and Kajiya algorithm
	// step 1
	float tnear = FLT_MIN;
	float tfar = FLT_MAX;
	vec3 origin = ray.origin;
	vec3 direction = ray.direction;

	float v0, vd, vmin, vmax;
	// step 2
	for (int i = 0; i < 3; i++) {
		v0 = origin[i];
		vd = direction[i];
		vmin = min[i];
		vmax = max[i];
		// step 3
		if (vd == 0) {
			if (v0 < vmin || v0 > vmax) {
				return false;
			}
		}
		else {
			continue;
		}
		// step 4
		float tmin, tmax;
		tmin = (vmin - v0) / vd;
		tmax = (vmax - v0) / vd;
		if (tmin > tmax) {
			float temp = tmin;
			tmin = tmax;
			tmax = temp;
		}
		
		if (tmin > tnear)
			tnear = tmin;
		if (tmax < tfar)
			tfar = tmax;
		// step 5
		if (tnear > tfar)
			return false;
		// step 6
		if (tfar < 0)
			return false;
		// step 7
		continue;

	}
	if (tnearp != nullptr)
		*tnearp = tnear;
	if (tfarp != nullptr)
		*tfarp = tfar;
	if (hitnear != nullptr)
		*hitnear = origin + direction * tnear;
	if (hitfar != nullptr)
		*hitfar = origin + direction * tfar;
	return true;

}