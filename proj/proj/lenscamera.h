#pragma once
#include "camera.h"
#include "vec.h"
#include "plane.h"
#include "ray.h"

struct lenscamera : public Camera {

	Plane focalPlane;
	float aperture;


	lenscamera(vec3 &eye, vec3 &at, vec3 &up, float fovy, float neard, float fard, int resX, 
		int resY, float focalDistance, float aperture) 
		: Camera(eye, at, up, fovy, neard, fard, resX, resY), 
		focalPlane(ze, focalDistance), aperture(aperture) {	
	}

	lenscamera(Camera* camera, float focalDistance, float aperture) : 
		Camera(camera->eye, camera->at, camera->up, camera->fovy, 
			camera->nearp, camera->farp, camera->resolutionX, camera->resolutionY),
		focalPlane(ze,focalDistance), aperture(aperture) {
	}

	vec3 getFocalPoint(Ray &r) {
		vec3 hitpoint;
		float t;
		focalPlane.CheckRayCollision(r, &t, &hitpoint);
		return hitpoint;
	}

	virtual vec3 getLenseSamplePoint() {

		float k1;
		float k2;		
		vec3 samplePoint;
		bool insideCircle = false;
		while (!insideCircle) {
			//random between [-aperture, +aperture]
			k1 = aperture * 2 * (((double)rand() / (RAND_MAX)) - 0.5); 
			k2 = aperture * 2 * (((double)rand() / (RAND_MAX)) - 0.5);

			samplePoint = eye + k1 * xe + k2 * ye;

			if ((samplePoint - eye).sqrMagnitude() < aperture * aperture) {
				insideCircle = true;
			}
		}
		
		return samplePoint;
	}

};