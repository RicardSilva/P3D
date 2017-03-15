#include "triangle.h"

#define EPSILON 0.000001

bool triangle::CheckRayCollision(const ray &ray, float *distance, vec3 *hitpoint) {
	const vec3 origin = ray.origin;
	const vec3 direction = ray.direction;
	vec3 v0v1 = this->point2 - this->point1;
	vec3 v0v2 = this->point3 - this->point1;
	vec3 pvec = CrossProduct(direction, v0v2);
	float det = DotProduct(v0v1, pvec);

#ifdef CULLING 
	// if the determinant is negative the triangle is backfacing
	// if the determinant is close to 0, the ray misses the triangle
	if (det < kEpsilon) return false;
#else 
	// ray and triangle are parallel if det is close to 0
	if (fabs(det) < EPSILON) return false;
#endif 
	float invDet = 1 / det;

	vec3 tvec = origin - point1;
	float u = DotProduct(tvec, pvec) * invDet;
	if (u < 0 || u > 1) 
		return false;

	vec3 qvec = CrossProduct(tvec, v0v1);
	float v = DotProduct(direction, qvec) * invDet;
	if (v < 0 || u + v > 1) 
		return false;

	float t = DotProduct(v0v2, qvec) * invDet;

	//collision is behind ray origin
	if (t < 0)
		return false;
	if (distance != nullptr)
		*distance = t;
	if (hitpoint != nullptr)
		*hitpoint = origin + direction * t;
	return true;
}
vec3 triangle::GetNormal(const ray &ray, const vec3 &point) {
	return normal;
}