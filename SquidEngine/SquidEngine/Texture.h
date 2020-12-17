#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#define TEXTURE_UNITS 32

#define NO_TEXTURE_PATH "";


extern int defaultTextureID;

class Texture {
protected:
	unsigned int textureID = 0;
	std::string imagePath;
	void createBlankTexture(int imgWidth, int imgHeight);
	void createColouredTexture(float red, float green, float blue, int imgWidth, int imgHeight);
public:
	Texture(int imgWidth, int imgHeight);
	Texture(float red, float green, float blue, int imgWidth, int imgHeight);
	Texture(const char* textureFile);
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

	void destroy();
};

