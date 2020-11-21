#pragma once

#include "SquidEngine.h"
#include "Window.h"
#include "Texture.h"
#include <list>



class FrameBuffer {
protected:
	unsigned int ID;
	unsigned int colourBufferID;
	unsigned int renderBufferID;
	int width, height;
	glm::vec3 clearCol;
	
	void initBuffer(int imgWidth, int imgHeight, glm::vec3& clearColour);

public:

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


class ScaledFrameBuffer : public FrameBuffer {
private:
	float percentWidth, percentHeight;
public:
	static std::list<ScaledFrameBuffer*> scaledFrameBuffers;
	ScaledFrameBuffer(ViewPort& view);
	ScaledFrameBuffer(float widthPercent, float heightPercent);
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