#pragma once
#include "material.h"
#include "vec.h"
#include "ray.h"
#include "light.h"
#include <stdlib.h>

class Object {
protected:
	material mat;

public:
	Object() {}
	Object(const material &mat) : mat(mat) {}

	float GetReflectance() { return mat.Ks; }
	float GetTransmittance() { return mat.t; }
	
	bool IsReflective() {
		return mat.Ks > 0;
	}
	bool IsTransmissive() {
		return mat.t > 0;
	}

	vec3 GetDiffuseColor(Light &light, vec3 &normal, vec3 &l);
	vec3 GetSpecularColor(Light &light, vec3 &normal, vec3 &l, vec3 &v);


	//RETURNS FIRST HIT POINT IF IT EXISTS
	virtual float GetEnterRefractionIndex(const Ray &ray) { return mat.refraction_index; }
	virtual float GetExitRefractionIndex(const Ray &ray) { return 1.0f; }
	virtual bool CheckRayCollision(const Ray &ray, float *distance, vec3 *hitpoint) = 0;
	virtual vec3 GetNormal(const Ray &ray, const vec3 &point) = 0;

};