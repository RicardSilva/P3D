#include "scene.h"

Scene::Scene() {
	backgroundColor = vec3(0.0f, 0.0f, 0.0f);
}
Scene::~Scene() {}

int Scene::LoadSceneNFF(std::string fileName) {
	
	std::ifstream ifile(fileName);
	if (ifile.fail()) {
		std::cerr << "Could not open file: " << fileName << std::endl;
		return -1;
	}
	std::string line;
	std::string keyword;
	std::stringstream sin;
	while (ifile.good()) {
		std::getline(ifile, line);
		sin = std::stringstream(line);
		sin >> keyword;

		if (keyword.compare("v") == 0) {
			std::getline(ifile, line);
			sin = std::stringstream(line);
			vec3 eye = vec3();
			vec3 at = vec3();
			vec3 up = vec3();
			float fovy;
			float near;
			int resX, resY;
			//EYE
			std::string junk;
			sin >> junk >> eye.x >> eye.y >> eye.z;
			std::getline(ifile, line);
			sin = std::stringstream(line);
			//AT
			sin >> junk >> at.x >> at.y >> at.z;
			std::getline(ifile, line);
			sin = std::stringstream(line);
			//UP
			sin >> junk >> up.x >> up.y >> up.z;
			std::getline(ifile, line);
			sin = std::stringstream(line);
			//FOVY
			sin >> junk >> fovy;
			std::getline(ifile, line);
			sin = std::stringstream(line);
			//NEAR
			sin >> junk >> near;
			std::getline(ifile, line);
			sin = std::stringstream(line);
			//RESOLUTION
			sin >> junk >> resX >> resY;

			this->cam = camera(eye, at, up, fovy, near, 1000, resX, resY);
			
		}
		else if (keyword.compare("b") == 0) {
			ParseBackground(sin);
		}
		else if (keyword.compare("l") == 0) ParseLight(sin);
		else if (keyword.compare("f") == 0) ParseMaterial(sin);
		else if (keyword.compare("c") == 0) {
			ParseCylinder(sin);
			std::getline(ifile, line);
			ParseCylinderBase(std::stringstream(line));
			std::getline(ifile, line);
			ParseCylinderApex(std::stringstream(line));
		}
		else if (keyword.compare("s") == 0) ParseSphere(sin);
		else if (keyword.compare("p") == 0) {
			int total_vertices;
			sin >> total_vertices;
			for (int i = 0; i < total_vertices; i++) {
				std::getline(ifile, line);
				ParsePolygon(std::stringstream(line));
			}
		}
		else if (keyword.compare("pp") == 0)
		{
			int total_vertices;
			sin >> total_vertices;
			for (int i = 0; i < total_vertices; i++) {
				std::getline(ifile, line);
				ParsePolygonPatch(std::stringstream(line));
			}
		}
		else if (keyword.compare("pl") == 0) ParsePlane(sin);

	}
	return 0;
}

void Scene::ParseBackground(std::stringstream& sin) {
	sin >> backgroundColor.x >> backgroundColor.y >> backgroundColor.z;
}
void Scene::ParseLight(std::stringstream& sin) {
	light l;
	sin >> l.position.x >> l.position.y >> l.position.z;
	if (!sin.eof())
		sin >> l.color.x >> l.color.y >> l.color.z;
	else
		l.color = vec3(1.0f, 1.0f, 1.0f);
	lights.push_back(l);
}
void Scene::ParseMaterial(std::stringstream& sin) {
	sin >> mat.color.x >> mat.color.y >> mat.color.z >> mat.Kd >> mat.Ks >> mat.shine >> mat.t >> mat.refraction_index;
}
void Scene::ParseCylinder(std::stringstream& sin) {
	std::cerr << "Not implemented: " << __FUNCTION__ << std::endl;
}
void Scene::ParseCylinderBase(std::stringstream& sin) {
	std::cerr << "Not implemented: " << __FUNCTION__ << std::endl;
}
void Scene::ParseCylinderApex(std::stringstream& sin) {
	std::cerr << "Not implemented: " << __FUNCTION__ << std::endl;
}
void Scene::ParseSphere(std::stringstream& sin) {
	sphere s = sphere();
	sin >> s.pos.x >> s.pos.y >> s.pos.z >> s.radius;
	s.mat = mat;
	objects.push_back(s);
}
void Scene::ParsePolygon(std::stringstream& sin) {
	std::cerr << "Not implemented: " << __FUNCTION__ << std::endl;
}
void Scene::ParsePolygonPatch(std::stringstream& sin) {
	std::cerr << "Not implemented: " << __FUNCTION__ << std::endl;
}
void Scene::ParsePlane(std::stringstream& sin) {
	plane p;
	vec3 point;
	sin >> point.x >> point.y >> point.z;
	p.point1 = point;
	sin >> point.x >> point.y >> point.z;
	p.point2 = point;
	sin >> point.x >> point.y >> point.z;
	p.point3 = point;
	p.mat = mat;
	objects.push_back(p);
}