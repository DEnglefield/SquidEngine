
#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int defaultTextureID = 0;

Texture::Texture() { initTexture(); }
Texture::Texture(const char* textureFile) {
	initTexture();
	openFile(textureFile);
}
unsigned char* Texture::getImage() { return imageData; }
unsigned int Texture::getID() { return textureID; }

bool Texture::openFile(const char* fileName) {
	bool success = true;
	glBindTexture(GL_TEXTURE_2D, textureID);
	stbi_set_flip_vertically_on_load(true);
	imageData = stbi_load(fileName, &width, &height, &numColourChannels, 0);
	if (imageData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
		success = false;
	}
	glBindTexture(GL_TEXTURE_2D, defaultTextureID);
	return success;
}

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