#pragma once

#include <iostream>
#include <string>
#include <SOIL.h>
#include "GL/glew.h"
#include "GL/freeglut.h"

#define TGA_RLE		10

namespace engine {


	class Texture
	{
	protected:
		GLuint texture;

	public:
		Texture() {}
		Texture(const std::string &name) {
			Create(name);
		}
		virtual ~Texture() {
			glDeleteTextures(1, &texture);
		}

		GLuint TextureID() { return texture; }

		virtual void Use() {
			glBindTexture(GL_TEXTURE_2D, texture);
		}
		virtual void UnUse() {
			glBindTexture(GL_TEXTURE_2D, 0);
		}

	private:
		void Create(std::string name);
		unsigned char *Load_TGA(const char *name, int* w, int *h, int *textureType);
	};

}