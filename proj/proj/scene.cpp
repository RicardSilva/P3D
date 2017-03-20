#include "scene.h"


bool Scene::LoadSceneNFF(std::string fileName) {
	std::ifstream ifile(fileName);
	if (ifile.fail()) {
		std::cerr << "Could not open file: " << fileName << std::endl;
		return false;
	}
	std::string line;
	std::string keyword;
	std::stringstream sin;
	while (ifile.good()) {
		std::getline(ifile, line);
		sin = std::stringstream(line);
		sin >> keyword;

		if (keyword.compare("v") == 0) {
			vec3 eye = vec3();
			vec3 at = vec3();
			vec3 up = vec3();
			double fovy;
			double near;
			int resX, resY;
			std::string junk;
		
			//EYE
			std::getline(ifile, line);
			sin = std::stringstream(line);
			sin >> junk >> eye.x >> eye.y >> eye.z;
			//AT
			std::getline(ifile, line);
			sin = std::stringstream(line);
			sin >> junk >> at.x >> at.y >> at.z;
			//UP
			std::getline(ifile, line);
			sin = std::stringstream(line);		
			sin >> junk >> up.x >> up.y >> up.z;
			//FOVY
			std::getline(ifile, line);
			sin = std::stringstream(line);
			sin >> junk >> fovy;
			//NEAR
			std::getline(ifile, line);
			sin = std::stringstream(line);
			sin >> junk >> near;
			//RESOLUTION
			std::getline(ifile, line);
			sin = std::stringstream(line);
			sin >> junk >> resX >> resY;

			this->cam = camera(eye, at, up, fovy, near, 1000, resX, resY);
			
		}
		else if (keyword.compare("b") == 0) ParseBackground(sin);
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
			std::vector<vec3> points;
			vec3 point = vec3();
			for (int i = 0; i < total_vertices; i++) {
				
				std::getline(ifile, line);
				sin = std::stringstream(line);
				sin >> point.x >> point.y >> point.z;
				points.push_back(point);
			}
			if (points.size() == 3) {
				Triangle* t = new Triangle(points, this->mat);
				objects.push_back(t);
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
	return true;
}

void Scene::ParseBackground(std::stringstream& sin) {
	vec3 backColor = vec3();
	sin >> backColor.x >> backColor.y >> backColor.z;
	backgroundColor = backColor;
}
void Scene::ParseLight(std::stringstream& sin) {
	vec3 position = vec3();
	vec3 color = vec3();
	sin >> position.x >> position.y >> position.z;
	if (!sin.eof())
		sin >> color.x >> color.y >> color.z;
	else
		color = vec3(1.0f, 1.0f, 1.0f);
	Light *l = new Light(position, color);
	lights.push_back(l);
}
void Scene::ParseMaterial(std::stringstream& sin) {
	vec3 color = vec3();
	double Kd, Ks, shine, t, refraction_index;
	sin >> color.x >> color.y >> color.z >> Kd >> Ks >> shine >> t >> refraction_index;
	this->mat = material(color, Kd, Ks, shine, t, refraction_index);
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
	vec3 centre = vec3();
	double radius;
	sin >> centre.x >> centre.y >> centre.z >> radius;
	Sphere *s = new Sphere(centre, radius, this->mat);
	objects.push_back(s);
}
void Scene::ParsePolygonPatch(std::stringstream& sin) {
	std::cerr << "Not implemented: " << __FUNCTION__ << std::endl;
}
void Scene::ParsePlane(std::stringstream& sin) {
	vec3 point1 = vec3();
	vec3 point2 = vec3();
	vec3 point3 = vec3();
	sin >> point1.x >> point1.y >> point1.z;
	sin >> point2.x >> point2.y >> point2.z;
	sin >> point3.x >> point3.y >> point3.z;
	Plane *p = new Plane(point1, point2, point3, this->mat);
	objects.push_back(p);
}