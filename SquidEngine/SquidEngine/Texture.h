#pragma once

#include "SquidEngine.h"

#define TEXTURE_UNITS 32
#define DEFAULT_TEXTURE 0

class Texture {
protected:
	unsigned char* imageData;
	int width = 0;
	int height = 0;
	int numColourChannels = 0;
	unsigned int textureID = 0;

	void initTexture();

public:
	Texture();
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
};