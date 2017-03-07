#include "Camera.h"


Ray camera::getPrimaryRay(int x, int y) {
	
	vec3 direction = -df * ze + h * (((y + 0.5f) / resolutionY) - 0.5f) * ye + w * (((x + 0.5f)/ resolutionX) - 0.5f) * xe;
	direction.Normalize();
	vec3 origin = eye;

	return Ray(origin, direction);
}
