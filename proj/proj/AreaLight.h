#pragma 
#include "light.h"
#include "vec.h"

struct AreaLight : public Light {
	vec3 a, b;
	
	AreaLight(const vec3 &position, const vec3 &color) : Light(position, color) {
	
		vec3 normal = vec3(0,0,0) - position; // points light to origin
		normal.Normalize();
		vec3 point = vec3();
		if (normal.x != 0 && normal.y != 0) {
			point.z = -(position.x * normal.x + position.y * normal.y) / normal.z + position.z;
		}
		else if (normal.x != 0 && normal.z != 0) {
			point.y = -(position.x * normal.x + position.z * normal.z) / normal.y + position.y;
		}
		else if (normal.y != 0 && normal.z != 0) {
			point.x = -(position.y * normal.y + position.z * normal.z) / normal.x + position.x;
		}
		a = point - position;
		a.Normalize();
		b = CrossProduct(normal, a);
		b.Normalize();	
	}

	AreaLight(Light l) : AreaLight(l.position, l.color) {};

	vec3 ComputeL(const vec3 &hitpoint) {
		float k1 = ((double)rand() / (RAND_MAX)) - 0.5f;
		float k2 = ((double)rand() / (RAND_MAX)) - 0.5f;
		vec3 randomPosition = position + k1 * a + k2 * b;
		vec3 l = randomPosition - hitpoint;
		l.Normalize();
		return l;
	}
};