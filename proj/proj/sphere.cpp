#include "sphere.h"

float Sphere::GetEnterRefractionIndex(const Ray &ray) {
	float index;

	//square of the distance between Ray origin and Sphere centre
	float d = (centre - ray.origin).sqrMagnitude();

	//Ray origin outside of Sphere
	if (d >= radius * radius)
		index = mat.refraction_index;
	//Ray origin inside of Sphere
	else
		index = 1.0f;

	return index;

}
float Sphere::GetExitRefractionIndex(const Ray &ray) {
	float index;

	//square of the distance between Ray origin and Sphere centre
	float d = (centre - ray.origin).sqrMagnitude();

	//Ray origin outside of Sphere
	if (d >= radius * radius)
		index = mat.refraction_index;
	//Ray origin inside of Sphere
	else
		index = 1.0f;

	return index;

}
bool Sphere::CheckRayCollision(const Ray &Ray, float *distance, vec3 *hitpoint) {
	const vec3 origin = Ray.origin;
	const vec3 direction = Ray.direction;
	const float radiusSqr = radius * radius;

	//square of the distance between Ray origin and Sphere centre
	float d = (centre - origin).sqrMagnitude();
	if (d == radiusSqr)
		return false;
	
	float b = direction.x * (centre.x - origin.x)
			+ direction.y * (centre.y - origin.y)
			+ direction.z * (centre.z - origin.z);

	if (d > radiusSqr && b < 0)
		return false;

	float r = (b * b) - d + radiusSqr;
	if (r < 0)
		return false;

	float t;
	if (d > radiusSqr)
		t = b - sqrt(r);
	else
		t = b + sqrt(r);
	if (distance != nullptr)
		*distance = t;
	if (hitpoint != nullptr)
		*hitpoint = origin + direction * t;
	return true;
	
}
vec3 Sphere::GetNormal(const Ray &Ray, const vec3 &point) {
	vec3 normal;

	//square of the distance between Ray origin and Sphere centre
	float d = (centre - Ray.origin).sqrMagnitude();
	
	//Ray origin outside of Sphere
	if(d >= radius * radius)
		normal = (point - centre) / radius;
	//Ray origin inside of Sphere
	else 
		normal = -1 * (point - centre) / radius;

	return normal;
}

void Sphere::InitializeBoundingBox() {
	vec3 min = vec3(centre.x - radius, centre.y - radius, centre.z - radius);
	vec3 max = vec3(centre.x + radius, centre.y + radius, centre.z + radius);
	bb = BoundingBox(min, max);
}