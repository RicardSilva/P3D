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

		if (keyword == "v") {
			std::getline(ifile, line);
			ParseFrom(std::stringstream(line));
			std::getline(ifile, line);
			ParseAt(std::stringstream(line));
			std::getline(ifile, line);
			ParseUp(std::stringstream(line));
			std::getline(ifile, line);
			ParseAngle(std::stringstream(line));
			std::getline(ifile, line);
			ParseHither(std::stringstream(line));
			std::getline(ifile, line);
			ParseResolution(std::stringstream(line));
		}
		else if (keyword == "b") ParseBackground(sin);
		else if (keyword == "l") ParseLight(sin);
		else if (keyword == "f") ParseMaterial(sin);
		else if (keyword == "c") {
			ParseCylinder(sin);
			std::getline(ifile, line);
			ParseCylinderBase(std::stringstream(line));
			std::getline(ifile, line);
			ParseCylinderApex(std::stringstream(line));
		}
		else if (keyword == "s") ParseSphere(sin);
		else if (keyword == "p") {
			int total_vertices;
			sin >> total_vertices;
			for (int i = 0; i < total_vertices; i++) {
				std::getline(ifile, line);
				ParsePolygon(std::stringstream(line));
			}
		}
		else if (keyword == "pp") 
		{
			int total_vertices;
			sin >> total_vertices;
			for (int i = 0; i < total_vertices; i++) {
				std::getline(ifile, line);
				ParsePolygonPatch(std::stringstream(line));
			}
		}
		else if (keyword == "pl") ParsePlane(sin);

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
void Scene::ParseFrom(std::stringstream& sin) {
	std::string junk;
	sin >> junk >> cam.from.x >> cam.from.y >> cam.from.z;
}
void Scene::ParseAt(std::stringstream& sin) {
	std::string junk;
	sin >> junk >> cam.at.x >> cam.at.y >> cam.at.z;
}
void Scene::ParseUp(std::stringstream& sin) {
	std::string junk;
	sin >> junk >> cam.up.x >> cam.up.y >> cam.up.z;
}
void Scene::ParseAngle(std::stringstream& sin) {
	std::string junk;
	sin >> junk >> cam.angle;
}
void Scene::ParseHither(std::stringstream& sin) {
	std::string junk;
	sin >> junk >> cam.hither;
}
void Scene::ParseResolution(std::stringstream& sin) {
	std::string junk;
	sin >> junk >> cam.resolutionX >> cam.resolutionY;
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
	sphere s;
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