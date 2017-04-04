#pragma once
#include "camera.h"
#include "vec.h"
#include "plane.h"
#include "ray.h"

struct lenscamera : public camera {

	Plane focalPlane;
	float aperture;


	lenscamera(vec3 &eye, vec3 &at, vec3 &up, float fovy, float neard, float fard, int resX, 
		int resY, float focalDistance, float aperture) 
		: camera(eye, at, up, fovy, neard, fard, resX, resY), 
		focalPlane(ze, focalDistance), aperture(aperture) {	
	}

	lenscamera(camera* camera, float focalDistance, float aperture) : 
		camera(camera->eye, camera->at, camera->up, camera->fovy, 
			camera->nearp, camera->farp, camera->resolutionX, camera->resolutionY),
		focalPlane(ze, focalDistance), aperture(aperture) {}

	vec3 getFocalPoint(Ray &r) {
		vec3 hitpoint;
		focalPlane.CheckRayCollision(r, nullptr, &hitpoint);
		return hitpoint;
	}

	vec3 getLenseSamplePoint() {

		float k1;
		float k2;		
		vec3 samplePoint;
		bool insideCircle = false;
		while (!insideCircle) {
			k1 = ((double)rand() / (RAND_MAX)) - 0.5;
			k2 = ((double)rand() / (RAND_MAX)) - 0.5;
			samplePoint = eye + k1 * xe + k2 * ye;

			if ((samplePoint - eye).sqrMagnitude() < aperture * aperture) {
				insideCircle = true;
			}
		}
		
		return samplePoint;
	}

};