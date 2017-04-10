#pragma once
#include "vec.h"
#include "Object.h"
#include "Ray.h"

class Plane : public Object {

	vec3 normal;
	float d;

public:
	Plane(const vec3 &point1, const vec3 &point2, const vec3 &point3, const material &mat) 
		: Object(mat) {

		vec3 n;
		float z;
		vec3 v1, v2;

		v1 = point2 - point1;
		v2 = point3 - point1;

		n = CrossProduct(v1, v2);
		n.Clean();
		z = -DotProduct(point1, n);

		float magnitude = n.Magnitude();

		normal = n / magnitude;
		d = z / magnitude;

		InitializeBoundingBox();

	}

	virtual bool Object::CheckRayCollision(const Ray &Ray, float *distance, vec3 *hitpoint);
	virtual vec3 GetNormal(const Ray &Ray, const vec3 &point);

private:
	void InitializeBoundingBox();
};