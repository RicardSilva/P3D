#pragma once
#include "engine.h"
#include <vector>
#include "Texture.h"
#include "Material.h"

using namespace std;

namespace engine {

	class SceneNode {

	protected:
		vector<SceneNode*> nodes;
		Mesh *mesh;
		mat4 matrix;
		vec3 color;
		Shader *shader;
		Texture *texture;
		Material *material;

	public:

		SceneNode() {
			this->shader = ShaderManager::Instance()->GetShader("defaultShader");
		}
		virtual ~SceneNode() {
			for (auto node : nodes)
				delete(node);
		}
		void setMatrix(mat4 &m) { matrix = m; }
		mat4 getMatrix() { return matrix; }

		void setMesh(Mesh* m) { mesh = m; }
		Mesh *getMesh() { return mesh; }

		void setMaterial(Material* m) { material = m; }
		Material getMaterial(){ return *material; }

		void setColor(vec3& v) { color = v; }
		void setTexture(Texture* t) { texture = t; }

		virtual void setShader(Shader* s) { shader = s; }
		Shader *getShader() { return shader; }

		void addNode(SceneNode *node) {
			nodes.push_back(node);
		}

		virtual void draw(mat4 &modelMatrix) {
			//draw this
			
			if (mesh != nullptr) {
				shader->Use();
				shader->LoadModelMatrix(modelMatrix * matrix);
				
				shader->LoadColor(color);
				if (texture != nullptr) {

					shader->LoadTexture(texture);
					glActiveTexture(GL_TEXTURE0);
					texture->Use();
				}
				mesh->draw();
				if (texture != nullptr)
					texture->UnUse();

				shader->UnUse();
				
			}


			//draw children
			for (auto node : nodes)
				node->draw(modelMatrix * matrix);
		}

		
	};

}