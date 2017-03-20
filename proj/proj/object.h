#pragma once
#include "material.h"
#include "vec.h"
#include "ray.h"
#include "light.h"
#include <stdlib.h>

class object {
public:
	material mat;

public:
	object() {}
	object(const material &mat) : mat(mat) {}


	vec3 GetAmbientColor();
	vec3 GetDiffuseColor(light &light, vec3 &normal, vec3 &l);
	vec3 GetSpecularColor(light &light, vec3 &normal, vec3 &l, vec3 &v);

	float getReflectionCoeficient();

	bool isReflective();
	bool isTransmissive();

	//RETURNS FIRST HIT POINT IF IT EXISTS
	virtual bool CheckRayCollision(const ray &ray, float *distance, vec3 *hitpoint) = 0;
	virtual vec3 GetNormal(const ray &ray, const vec3 &point) = 0;

};