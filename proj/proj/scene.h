#pragma once
#include "camera.h"
#include "material.h"
#include "light.h"
#include "object.h"
#include "sphere.h"
#include "plane.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

class Scene {
private:
	vec3 backgroundColor;
	camera cam;
	material mat;
	std::vector <light> lights;
	std::vector <object> objects;


public:
	Scene();
	virtual ~Scene();
	int LoadSceneNFF(std::string fileName);

	void ParseBackground(std::stringstream& sin);
	void ParseLight(std::stringstream& sin);
	void ParseMaterial(std::stringstream& sin);
	void ParseFrom(std::stringstream& sin);
	void ParseAt(std::stringstream& sin);
	void ParseUp(std::stringstream& sin);
	void ParseAngle(std::stringstream& sin);
	void ParseHither(std::stringstream& sin);
	void ParseResolution(std::stringstream& sin);
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

};

