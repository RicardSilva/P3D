#include "Texture.h"

namespace engine {

	bool isOpenGLError() {
		bool isError = false;
		GLenum errCode;
		const GLubyte *errString;
		while ((errCode = glGetError()) != GL_NO_ERROR) {
			isError = true;
			errString = gluErrorString(errCode);
			std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
		}
		return isError;
	}
	void checkOpenGLError(std::string error)
	{
		if (isOpenGLError()) {
			std::cerr << error << std::endl;
			getchar();
			exit(EXIT_FAILURE);
		}
	}

	void Texture::Create(std::string name)
	{
		glGenTextures(1, &texture);
		int width, height;
		int textureType;
		unsigned char* image;

		glActiveTexture(GL_TEXTURE0);

		glBindTexture(GL_TEXTURE_2D, texture);

		if (name.substr(name.find_last_of(".") + 1) == "tga") {
			image = Load_TGA(name.c_str(), &width, &height, &textureType);
		}
		else {
			image = SOIL_load_image(name.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
			textureType = GL_RGBA;
		}


		

		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, textureType, width, height, 0, textureType, GL_UNSIGNED_BYTE, image);

		
		glBindTexture(GL_TEXTURE_2D, 0);
		SOIL_free_image_data(image);
		checkOpenGLError("ERROR: Could not create texture: " + name);

	}

	unsigned char *Texture::Load_TGA(const char *name, int* w, int *h, int *textureType) {

	
		unsigned char *data;
		FILE *pFile;
		int width = 0;
		int height = 0;
		GLubyte length = 0;
		GLubyte imgType = 0;
		GLubyte bits = 0;
		int channels = 0;
		int stride = 0;
		int i = 0;

		if ((pFile = fopen(name, "rb")) == NULL)
		{
			printf("Error loading tga file.\n");
			return NULL;
		}



		fread(&length, sizeof(GLubyte), 1, pFile);

		fseek(pFile, 1, SEEK_CUR);

		fread(&imgType, sizeof(GLubyte), 1, pFile);

		fseek(pFile, 9, SEEK_CUR);

		fread(&width, sizeof(WORD), 1, pFile);
		fread(&height, sizeof(WORD), 1, pFile);
		fread(&bits, sizeof(GLubyte), 1, pFile);

		fseek(pFile, length + 1, SEEK_CUR);
		//printf("bits=%d\n",bits);

		if (imgType != TGA_RLE)
		{
			// Check for 24 or 32 Bit
			if (bits == 24 || bits == 32)
			{

				channels = bits / 8;
				stride = channels * width;
				data = new unsigned char[stride * height];

				for (int y = 0; y < height; y++)
				{
					unsigned char *pLine = &(data[stride * y]);

					fread(pLine, stride, 1, pFile);

					for (i = 0; i < stride; i += channels)
					{
						int temp = pLine[i];
						pLine[i] = pLine[i + 2];
						pLine[i + 2] = temp;
					}
				}
			}

			// Check for 16 Bit
			else if (bits == 16)
			{
				unsigned short pixels = 0;
				int r = 0, g = 0, b = 0;

				channels = 3;
				stride = channels * width;
				data = new unsigned char[stride * height];

				for (int i = 0; i < width*height; i++)
				{
					fread(&pixels, sizeof(unsigned short), 1, pFile);

					b = (pixels & 0x1f) << 3;
					g = ((pixels >> 5) & 0x1f) << 3;
					r = ((pixels >> 10) & 0x1f) << 3;

					data[i * 3 + 0] = r;
					data[i * 3 + 1] = g;
					data[i * 3 + 2] = b;
				}
			}

			else
				return NULL;
		}

		else
		{

			GLubyte rleID = 0;
			int colorsRead = 0;
			channels = bits / 8;
			stride = channels * width;

			data = new unsigned char[stride * height];
			GLubyte *pColors = new GLubyte[channels];

			while (i < width*height)
			{

				fread(&rleID, sizeof(GLubyte), 1, pFile);


				if (rleID < 128)
				{
					rleID++;

					while (rleID)
					{
						fread(pColors, sizeof(GLubyte) * channels, 1, pFile);

						data[colorsRead + 0] = pColors[2];
						data[colorsRead + 1] = pColors[1];
						data[colorsRead + 2] = pColors[0];

						if (bits == 32)	data[colorsRead + 3] = pColors[3];

						i++;
						rleID--;
						colorsRead += channels;
					}
				}

				else
				{
					rleID -= 127;

					fread(pColors, sizeof(GLubyte) * channels, 1, pFile);

					while (rleID)
					{
						data[colorsRead + 0] = pColors[2];
						data[colorsRead + 1] = pColors[1];
						data[colorsRead + 2] = pColors[0];

						if (bits == 32)	data[colorsRead + 3] = pColors[3];

						i++;
						rleID--;
						colorsRead += channels;
					}
				}
			}
			delete[] pColors;
		}

		fclose(pFile);


	
		*textureType = GL_RGB;
		if (channels == 4)	*textureType = GL_RGBA;
		*w = width;
		*h = height;

		return data;


	}


}