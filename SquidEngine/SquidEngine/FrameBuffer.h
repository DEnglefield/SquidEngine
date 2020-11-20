#pragma once

#include "SquidEngine.h"
#include "Window.h"
#include "Texture.h"
#include <vector>



class FrameBuffer {
protected:
	unsigned int ID;
	unsigned int colourBufferID;
	unsigned int renderBufferID;
	int width, height;
	glm::vec3 clearCol;
	
	void initBuffer(int imgWidth, int imgHeight, glm::vec3& clearColour);

public:

	static std::vector<FrameBuffer*> scaledFrameBuffers;
	unsigned int getTextureOutput();

	FrameBuffer() {};
	FrameBuffer(GLFWwindow* window);
	FrameBuffer(int imgWidth, int imgHeight);
	FrameBuffer(int imgWidth, int imgHeight, glm::vec3& clearColour);

	void setClearColour(glm::vec3 clearColour);
	void use();
	void use(struct ViewPort& viewPort);

	int getWidth();
	int getHeight();

	void resizeBuffer(int imgWidth, int imgHeight);
	void destroy();
};


class DrawFrameBuffer {
private:
	glm::vec3 clearCol;
public:
	DrawFrameBuffer();
	void use();
	void use(struct ViewPort& viewPort);

	int getWidth();
	int getHeight();
	void setClearColour(glm::vec3 clearColour);
};


void resizeFrameBuffers(int width, int height);