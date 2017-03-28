#include "Camera.h"


Ray camera::getPrimaryRay(int x, int y) {
	
	vec3 direction = -df * ze + h * (((y + 0.5f) / resolutionY) - 0.5f) * ye + w * (((x + 0.5f)/ resolutionX) - 0.5f) * xe;
	direction.Normalize();
	vec3 origin = eye;

	return Ray(origin, direction);
}

Ray camera::getRandomPrimaryRay(int x, int y) { 
	double rand_y = ((rand() /(double) RAND_MAX) - 0.5);
	double rand_x = ((rand() /(double) RAND_MAX) - 0.5);

	vec3 direction = -df * ze + h * (((y + 0.5f + rand_y) / resolutionY) - 0.5f) * ye + w * (((x + 0.5f + rand_x) / resolutionX) - 0.5f) * xe;
	direction.Normalize();
	vec3 origin = eye;

	return Ray(origin, direction);
}

Ray camera::getJitteredPrimaryRay(int x, int y, int p, int q, int n) {
	double rand_p = ((rand() / (double)RAND_MAX)); 
	double rand_q = ((rand() / (double)RAND_MAX)); 

	vec3 direction = -df * ze 
		+ h * (((y + (q + rand_q)/n) / resolutionY) - 0.5f) * ye 
		+ w * (((x + (p + rand_p)/n) / resolutionX) - 0.5f) * xe;
	direction.Normalize();
	vec3 origin = eye;

	return Ray(origin, direction);
}

