#include "Plane.h" 

bool Plane::CheckRayCollision(const Ray &Ray, float *distance, vec3 *hitpoint) {

	if (Ray.id == lastRay) {
		if (distance != nullptr)
			*distance = lastT;
		if (hitpoint != nullptr)
			*hitpoint = lastHitpoint;
		return true;
	}
		

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
	vec3 hp = origin + direction * t;
	lastRay = Ray.id;
	lastHitpoint = hp;
	lastT = t;
	
	if (distance != nullptr)
		*distance = t;
	if (hitpoint != nullptr)
		*hitpoint = hp;
	return true;

}
vec3 Plane::GetNormal(const Ray &Ray, const vec3 &point) {
	return normal;
}

void Plane::InitializeBoundingBox() {}