#include "Plane.h"

bool Plane::CheckRayCollision(const Ray &Ray, float *distance, vec3 *hitpoint) {
	vec3 origin = Ray.origin;
	vec3 direction = Ray.direction;
	float t;
	float dotProductNormalDirection = DotProduct(normal, direction);
	//Plane and Ray are parallel
	if (dotProductNormalDirection == 0)
		return false;
	t = -1 * ((DotProduct(normal, origin) + d) / dotProductNormalDirection);
	
	//collision is behind Ray origin
	if (t < 0)
		return false;
	if (distance != nullptr)
		*distance = t;
	if (hitpoint != nullptr)
		*hitpoint = origin + direction * t;
	return true;

}
vec3 Plane::GetNormal(const Ray &Ray, const vec3 &point) {
	return normal;
}