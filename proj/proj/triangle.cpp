#include "Triangle.h"

#define EPSILON 0.000001

bool Triangle::CheckRayCollision(const Ray &Ray, float *distance, vec3 *hitpoint) {

	if (Ray.id == lastRay) {
		*distance = lastT;
		*hitpoint = lastHitpoint;
		return true;
	}

	//Implementation of Tomas Moller algorithm
    //http://www.cs.virginia.edu/~gfx/courses/2003/ImageSynthesis/papers/Acceleration/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf
	const vec3 origin = Ray.origin;
	const vec3 direction = Ray.direction;
	vec3 v0v1 = this->point2 - this->point1;
	vec3 v0v2 = this->point3 - this->point1;
	vec3 pvec = CrossProduct(direction, v0v2);
	float det = DotProduct(v0v1, pvec);


	// Ray and Triangle are parallel if det is close to 0
	if (fabs(det) < EPSILON) return false;

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

	//collision is behind Ray origin
	if (t < 0)
		return false;
	vec3 hp = origin + direction * t;
	lastRay = Ray.id;
	lastHitpoint = hp;
	lastT = t;

	*distance = t;
	*hitpoint = hp;
	return true;
}
bool Triangle::CheckRayCollision(const Ray &Ray) {

	if (Ray.id == lastRay) {
		return true;
	}

	//Implementation of Tomas Moller algorithm
	//http://www.cs.virginia.edu/~gfx/courses/2003/ImageSynthesis/papers/Acceleration/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf
	const vec3 origin = Ray.origin;
	const vec3 direction = Ray.direction;
	vec3 v0v1 = this->point2 - this->point1;
	vec3 v0v2 = this->point3 - this->point1;
	vec3 pvec = CrossProduct(direction, v0v2);
	float det = DotProduct(v0v1, pvec);


	// Ray and Triangle are parallel if det is close to 0
	if (fabs(det) < EPSILON) return false;

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

	//collision is behind Ray origin
	if (t < 0)
		return false;

	lastRay = Ray.id;
	return true;
}
vec3 Triangle::GetNormal(const Ray &Ray, const vec3 &point) {
	return normal;
}

void Triangle::InitializeBoundingBox(std::vector<vec3> points) {
	vec3 min = vec3(FLT_MAX, FLT_MAX, FLT_MAX);
	vec3 max = vec3(FLT_MIN, FLT_MIN, FLT_MIN);
	for (auto &point : points) {
		if (point.x < min.x)
			min.x = point.x;
		if (point.x > max.x)
			max.x = point.x;
		if (point.y < min.y)
			min.y = point.y;
		if (point.y > max.y)
			max.y = point.y;
		if (point.z < min.z)
			min.z = point.z;
		if (point.z > max.z)
			max.z = point.z;

	}
	bb = BoundingBox(min, max);
}