#include "object.h"

vec3 Object::GetAmbientColor() {
	return mat.color * mat.Kd;
}

vec3 Object::GetDiffuseColor(Light &Light, vec3 &normal, vec3 &l) {
	vec3 color = vec3();
	float dotProductNL = DotProduct(normal, l) * mat.Kd;
	
	color.x = Light.color.x * mat.color.x * dotProductNL;
	color.y = Light.color.y * mat.color.y * dotProductNL;
	color.z = Light.color.z * mat.color.z * dotProductNL;
	
	return color;
}

vec3 Object::GetSpecularColor(Light &Light, vec3 &normal, vec3 &l, vec3 &v) {
	vec3 color = vec3();
	vec3 r = 2 * DotProduct(v, normal) * normal - v;
	float dotProductRL = DotProduct(r, l);
	float power = 0;
	if (dotProductRL > 0)
		power = pow(DotProduct(r, l), mat.shine) * mat.Ks;

	color.x = Light.color.x * power;
	color.y = Light.color.y * power;
	color.z = Light.color.z * power;

	return color;
}

