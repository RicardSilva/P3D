#include "object.h"

vec3 object::GetAmbientColor() {
	return mat.color * mat.Kd;
}

vec3 object::GetDiffuseColor(light &light, vec3 &normal, vec3 &l) {
	vec3 color = vec3();
	float dotProductNL = DotProduct(normal, l) * mat.Kd;
	
	color.x = light.color.x * mat.color.x * dotProductNL;
	color.y = light.color.y * mat.color.y * dotProductNL;
	color.z = light.color.z * mat.color.z * dotProductNL;
	
	return color;
}

vec3 object::GetSpecularColor(light &light, vec3 &normal, vec3 &l, vec3 &v) {
	vec3 color = vec3();
	vec3 r = 2 * DotProduct(v, normal) * normal - v;
	float dotProductRL = DotProduct(r, l);
	float power = 0;
	if (dotProductRL > 0)
		power = pow(DotProduct(r, l), mat.shine) * mat.Ks;

	color.x = light.color.x * power;
	color.y = light.color.y * power;
	color.z = light.color.z * power;

	return color;
}