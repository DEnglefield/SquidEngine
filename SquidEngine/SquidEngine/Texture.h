#pragma once

#include "SquidEngine.h"

#define TEXTURE_UNITS 32
#define MAX_TEXTURES 30

#define DIFFUSE_MAP_TEXTURE_LAYER 0
#define SPECULAR_MAP_TEXTURE_LAYER 1

extern int defaultTextureID;

class Texture {
protected:
	unsigned char* imageData;
	int width = 0;
	int height = 0;
	int numColourChannels = 0;
	unsigned int textureID = 0;

	void createBlankTexture(float red, float green, float blue);
public:
	Texture();
	Texture(float red, float green, float blue, int imgWidth, int imgHeight);
	Texture(const char* textureFile);
	bool openFile(const char* fileName);
	unsigned char* getImage();

	unsigned int getID();

	void setScaleUpFilter(int filter);
	void setScaleDownFilter(int filter);
	void setWrappingX(int wrapper);
	void setWrappingY(int wrapper);
	void setBorderColour(glm::vec4 borderColour);

	void setWrapping(int wrapX, int wrapY);
	void setFiltering(int scaleUp, int scaleDown);

	void initTexture();
};

