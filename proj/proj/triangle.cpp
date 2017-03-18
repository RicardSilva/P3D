#include "Triangle.h"

#define EPSILON 0.000001

bool Triangle::CheckRayCollision(const Ray &Ray, float *distance, vec3 *hitpoint) {
	const vec3 origin = Ray.origin;
	const vec3 direction = Ray.direction;
	
	vec3 pvec = CrossProduct(direction, v2);
	float det = DotProduct(v1, pvec);


	// if the determinant is close to 0, the Ray misses the Triangle
	if (det < EPSILON) return false;

	float invDet = 1 / det;

	vec3 tvec = origin - point1;
	float u = DotProduct(tvec, pvec) * invDet;
	if (u < 0 || u > 1) 
		return false;

	vec3 qvec = CrossProduct(tvec, v1);
	float v = DotProduct(direction, qvec) * invDet;
	if (v < 0 || u + v > 1) 
		return false;

	float t = DotProduct(v2, qvec) * invDet;

	//collision is behind Ray origin
	if (t < 0)
		return false;
	if (distance != nullptr)
		*distance = t;
	if (hitpoint != nullptr)
		*hitpoint = origin + direction * t;
	return true;
}
vec3 Triangle::GetNormal(const Ray &Ray, const vec3 &point) {
	return normal;
}