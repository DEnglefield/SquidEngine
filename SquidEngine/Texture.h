#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#define TEXTURE_UNITS 32

#define NO_TEXTURE_PATH "";

#define TEXTURE_DIFFUSE_MAP 0
#define TEXTURE_SPECULAR_MAP 1
#define TEXTURE_COLOUR_BUFFER 2
#define TEXTURE_NORMAL_MAP 3


extern int defaultTextureID;

class Texture {
protected:
	unsigned int textureID = 0;
	std::string imagePath;
	int textureType;
	bool doGammarCorrection();
	void createTexImage(int width, int height, const void* imageData);
	void createBlankTexture(int imgWidth, int imgHeight);
	void createColouredTexture(float red, float green, float blue, int imgWidth, int imgHeight);
public:
	Texture(int imgWidth, int imgHeight, int type);
	Texture(float red, float green, float blue, int imgWidth, int imgHeight, int type);
	Texture(const char* textureFile, int type);
	bool openFile(const char* fileName);

	unsigned int getID();
	std::string getImagePath();

	void setScaleUpFilter(int filter);
	void setScaleDownFilter(int filter);
	void setWrappingX(int wrapper);
	void setWrappingY(int wrapper);
	void setBorderColour(glm::vec4 borderColour);

	void setWrapping(int wrapX, int wrapY);
	void setFiltering(int scaleUp, int scaleDown);

	void initTexture();

	inline int getTextureType() { return textureType; };

	void destroy();
};

