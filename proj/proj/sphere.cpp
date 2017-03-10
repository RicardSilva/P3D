#include "sphere.h"

bool sphere::CheckRayCollision(const ray &ray, float *distance, vec3 *hitpoint) {
	const vec3 origin = ray.origin;
	const vec3 direction = ray.direction;
	const float radiusSqr = radius * radius;

	//square of the distance between ray origin and sphere centre
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

	*distance = t;
	*hitpoint = origin + direction * t;
	return true;
	
}
vec3 sphere::ComputeNormal(const ray &ray, const vec3 &point) {
	vec3 normal;

	//square of the distance between ray origin and sphere centre
	float d = (centre - ray.origin).sqrMagnitude();
	
	//ray origin outside of sphere
	if(d >= radius * radius)
		normal = (point - centre) / radius;
	//ray origin inside of sphere
	else 
		normal = (point - centre) / radius * -1;

	return normal;
}