#pragma once
#include "material.h"
#include "vec.h"
#include "Ray.h"
#include "Light.h"
#include <stdlib.h>

class Object {

	material mat;

public:
	Object() {}
	Object(const material &mat) : mat(mat) {}

	float GetReflectance() { return mat.shine; }
	float GetTransmittance() { return mat.t; }
	float GetRefractionIndex() { return mat.refraction_index; }
	bool IsReflective() {
		return mat.shine > 0;
	}
	bool IsTransmissive() {
		return mat.t > 0;
	}

	vec3 GetDiffuseColor(Light &Light, vec3 &normal, vec3 &l);
	vec3 GetSpecularColor(Light &Light, vec3 &normal, vec3 &l, vec3 &v);


	//RETURNS FIRST HIT POINT IF IT EXISTS
	virtual bool CheckRayCollision(const Ray &Ray, float *distance, vec3 *hitpoint) = 0;
	virtual vec3 GetNormal(const Ray &Ray, const vec3 &point) = 0;

};