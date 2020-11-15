
#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <vector>

int defaultTextureID = 0;

Texture::Texture() { }

Texture::Texture(float red, float green, float blue, int imgWidth, int imgHeight) {
	imagePath = NO_TEXTURE_PATH;
	initTexture();
	createBlankTexture(red, green, blue, imgWidth, imgHeight);
}


Texture::Texture(const char* textureFile) {
	imagePath = textureFile;
	initTexture();
	openFile(textureFile);
}


unsigned int Texture::getID() { return textureID; }


void Texture::createBlankTexture(float red, float green, float blue, int imgWidth, int imgHeight) {
	glBindTexture(GL_TEXTURE_2D, textureID);
	int numColourChannels = 3;
	
	unsigned int imageSize = (imgWidth * imgHeight) * numColourChannels;

	unsigned char pixel[3]{ (unsigned char)(255 * red), (unsigned char)(255 * green) , (unsigned char)(255 * blue) };

	unsigned char* imageData = new unsigned char[imageSize];

	for (int i = 0; i < imageSize; i+=numColourChannels) {
		imageData[i] = pixel[0];
		imageData[i+1] = pixel[1];
		imageData[i+2] = pixel[2];
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	glBindTexture(GL_TEXTURE_2D, defaultTextureID);

	delete[] imageData;
}


bool Texture::openFile(const char* fileName) {
	bool success = true;
	glBindTexture(GL_TEXTURE_2D, textureID);
	stbi_set_flip_vertically_on_load(false);
	int width, height, numColourChannels;
	unsigned char* imageData = stbi_load(fileName, &width, &height, &numColourChannels, STBI_rgb_alpha);

	if (imageData) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	else {
		std::cout << "Failed to load texture: " << fileName << std::endl;
		success = false;
	}
	glBindTexture(GL_TEXTURE_2D, defaultTextureID);
	stbi_image_free(imageData);
	return success;
}


std::string Texture::getImagePath() { return imagePath;}


void Texture::initTexture() {
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	setWrapping(GL_REPEAT, GL_REPEAT);
	setFiltering(GL_NEAREST, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, defaultTextureID);
}

void Texture::setWrapping(int wrapX, int wrapY) {
	setWrappingX(wrapX);
	setWrappingY(wrapY);
}

void Texture::setFiltering(int scaleUp, int scaleDown) {
	setScaleUpFilter(scaleUp);
	setScaleDownFilter(scaleDown);
}


void Texture::setScaleUpFilter(int filter) {
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glBindTexture(GL_TEXTURE_2D, defaultTextureID);
}

void Texture::setScaleDownFilter(int filter) {
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glBindTexture(GL_TEXTURE_2D, defaultTextureID);
}

void Texture::setWrappingX(int wrapper) {
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapper);
	glBindTexture(GL_TEXTURE_2D, defaultTextureID);
}

void Texture::setWrappingY(int wrapper) {
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapper);
	glBindTexture(1, defaultTextureID);
}

void Texture::setBorderColour(glm::vec4 borderColour) {
	float colour[4] = { borderColour.x, borderColour.y, borderColour.z, borderColour.a };

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, colour);
	glBindTexture(GL_TEXTURE_2D, defaultTextureID);
}