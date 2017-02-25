#pragma once
#include "camera.h"

class Scene {
private:
	Camera* camera;

public:
	Scene();
	virtual ~Scene();
	int LoadSceneNFF(std::string fileName);

	void SetCamera(Camera *c) { camera = c; }
	Camera* GetCamera() { return camera; }

};

