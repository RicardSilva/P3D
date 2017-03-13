#include "plane.h"

bool plane::CheckRayCollision(const ray &ray, float *distance, vec3 *hitpoint) {
	vec3 origin = ray.origin;
	vec3 direction = ray.direction;
	float t;
	float dotProductNormalDirection = DotProduct(normal, direction);
	//plane and ray are parallel
	if (dotProductNormalDirection == 0)
		return false;
	t = -1 * ((DotProduct(normal, origin) + d) / dotProductNormalDirection);
	
	//collision is behind ray origin
	if (t < 0)
		return false;
	if (distance != nullptr)
		*distance = t;
	if (hitpoint != nullptr)
		*hitpoint = origin + direction * t;
	return true;

}
vec3 plane::GetNormal(const ray &ray, const vec3 &point) {
	return normal;
}