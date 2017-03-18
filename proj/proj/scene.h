#pragma once
#include "camera.h"
#include "material.h"
#include "light.h"
#include "object.h"
#include "sphere.h"
#include "plane.h"
#include "triangle.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>

class Scene {
private:
	vec3 backgroundColor;
	camera cam;
	material mat;
	std::vector <Light*> lights;
	std::vector <Object*> objects;


public:
	Scene() {
		backgroundColor = vec3(0.0f, 0.0f, 0.0f);
	}
	virtual ~Scene() {
		for (auto &object : objects) {
			delete object;
		}
		for (auto &light : lights) {
			delete light;
		}
	}

	bool LoadSceneNFF(std::string fileName);

	void ParseBackground(std::stringstream& sin);
	void ParseLight(std::stringstream& sin);
	void ParseMaterial(std::stringstream& sin);
	void ParseCylinder(std::stringstream& sin);
	void ParseCylinderBase(std::stringstream& sin);
	void ParseCylinderApex(std::stringstream& sin);
	void ParseSphere(std::stringstream& sin);
	void ParsePolygon(std::stringstream& sin);
	void ParsePolygonPatch(std::stringstream& sin);
	void ParsePlane(std::stringstream& sin);

	camera GetCamera() {
		return cam;
	}
	vec3 GetBackgroundColor() {
		return backgroundColor;
	}
	const std::vector<Object*> GetObjects() {
		return objects;
	}
	const std::vector<Light*> GetLights() {
		return lights;
	}
};

