
#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <glad\glad.h>
#include <iostream>
#include "GlobalConfig.h"


Texture::Texture(int imgWidth, int imgHeight, int type) {
	imagePath = NO_TEXTURE_PATH;
	textureType = type;
	initTexture();
	createBlankTexture(imgWidth, imgHeight);
}

Texture::Texture(float red, float green, float blue, int imgWidth, int imgHeight, int type) {
	imagePath = NO_TEXTURE_PATH;
	textureType = type;
	initTexture();
	createColouredTexture(red, green, blue, imgWidth, imgHeight);
}


Texture::Texture(const char* textureFile, int type) {
	imagePath = textureFile;
	textureType = type;
	initTexture();
	openFile(textureFile);
	
}


unsigned int Texture::getID() { return textureID; }


void Texture::createTexImage(int width, int height, const void* imageData) {
	glBindTexture(GL_TEXTURE_2D, textureID);
	if (doGammarCorrection()) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	}
	
	glBindTexture(GL_TEXTURE_2D, renderConfig.defaultTextureID);
}


void Texture::createBlankTexture(int imgWidth, int imgHeight) { createTexImage(imgWidth, imgHeight, NULL); }


void Texture::createColouredTexture(float red, float green, float blue, int imgWidth, int imgHeight) {
	glBindTexture(GL_TEXTURE_2D, textureID);
	int numColourChannels = 3;
	texWidth = imgWidth;
	texHeight = imgHeight;
	unsigned int imageSize = (texWidth * texHeight) * numColourChannels;

	unsigned char pixel[3]{ (unsigned char)(255 * red), (unsigned char)(255 * green) , (unsigned char)(255 * blue) };

	unsigned char* imageData = new unsigned char[imageSize];

	for (int i = 0; i < imageSize; i+=numColourChannels) {
		imageData[i] = pixel[0];
		imageData[i+1] = pixel[1];
		imageData[i+2] = pixel[2];
	}

	createTexImage(texWidth, texHeight, imageData);
	
	glBindTexture(GL_TEXTURE_2D, renderConfig.defaultTextureID);

	delete[] imageData;
}


bool Texture::openFile(const char* fileName) {
	bool success = true;
	glBindTexture(GL_TEXTURE_2D, textureID);
	stbi_set_flip_vertically_on_load(false);
	unsigned char* imageData = stbi_load(fileName, &texWidth, &texHeight, NULL, STBI_rgb_alpha);

	if (imageData) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		createTexImage(texWidth, texHeight, imageData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	else {
		std::cout << "Failed to load texture: " << fileName << std::endl;
		success = false;
	}
	glBindTexture(GL_TEXTURE_2D, renderConfig.defaultTextureID);
	stbi_image_free(imageData);
	return success;
}


std::string Texture::getImagePath() { return imagePath;}


void Texture::initTexture() {
	texWidth = 1;
	texHeight = 1;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	setWrapping(GL_REPEAT, GL_REPEAT);
	setFiltering(GL_NEAREST, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, renderConfig.defaultTextureID);
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
	glBindTexture(GL_TEXTURE_2D, renderConfig.defaultTextureID);
}

void Texture::setScaleDownFilter(int filter) {
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glBindTexture(GL_TEXTURE_2D, renderConfig.defaultTextureID);
}

void Texture::setWrappingX(int wrapper) {
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapper);
	glBindTexture(GL_TEXTURE_2D, renderConfig.defaultTextureID);
}

void Texture::setWrappingY(int wrapper) {
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapper);
	glBindTexture(1, renderConfig.defaultTextureID);
}

void Texture::setBorderColour(glm::vec4 borderColour) {
	float colour[4] = { borderColour.x, borderColour.y, borderColour.z, borderColour.a };

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, colour);
	glBindTexture(GL_TEXTURE_2D, renderConfig.defaultTextureID);
}


bool Texture::doGammarCorrection() {
	switch (textureType) {
	case TEXTURE_DIFFUSE_MAP: return false;
	case TEXTURE_SPECULAR_MAP: return true;
	case TEXTURE_NORMAL_MAP: return true;
	case TEXTURE_COLOUR_BUFFER: return true;
	}

	return true;
}


void Texture::saveToJPG(const char* fileName) {
	unsigned char* imageData = new unsigned char[4 * (texWidth * texHeight)];
	glBindTexture(GL_TEXTURE_2D, textureID);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	stbi_write_jpg("test.jpg", texWidth, texHeight, 4, imageData, 100);
}



void Texture::destroy() { glDeleteTextures(1, &textureID); };