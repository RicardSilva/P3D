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
	//return 1.0f;

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
	//return 1.0f;

}
bool Sphere::CheckRayCollision(const Ray &ray, float *distance, vec3 *hitpoint) {
	const vec3 origin = ray.origin;
	const vec3 direction = ray.direction;
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
vec3 Sphere::GetNormal(const Ray &ray, const vec3 &point) {
	vec3 normal;

	//square of the distance between Ray origin and Sphere centre
	float d = (centre - ray.origin).sqrMagnitude();
	
	//Ray origin outside of Sphere
	if(d > radius * radius)
		normal = (point - centre) / radius;
	//Ray origin inside of Sphere
	else 
		normal = (centre - point) / radius;

	return normal;
}