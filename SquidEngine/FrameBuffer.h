#pragma once

#include <glad\glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <list>
#include "Window.h"

class FrameBuffer {
protected:
	unsigned int ID;
	glm::vec3 clearCol;
	int width, height;
public:
	void use();
	void use(struct ViewPort& viewPort);
	void setClearColour(glm::vec3 clearColour);
	inline int getHeight() { return height; }
	inline int getWidth() { return width; }
	inline unsigned int getID() { return ID; };
	inline void destroy() {};
};

class StaticFrameBuffer : public FrameBuffer {
protected:
	
	unsigned int colourBufferID;
	unsigned int renderBufferID;
	
	void initBuffer(int imgWidth, int imgHeight, glm::vec3& clearColour);

public:

	unsigned int getTextureOutput();

	StaticFrameBuffer();
	StaticFrameBuffer(int imgWidth, int imgHeight);
	StaticFrameBuffer(int imgWidth, int imgHeight, glm::vec3& clearColour);

	void resizeBuffer(int imgWidth, int imgHeight);
	void destroy();
};


class ScaledFrameBuffer : public StaticFrameBuffer {
private:
	float percentWidth, percentHeight;
public:
	static std::list<ScaledFrameBuffer*> scaledFrameBuffers;
	ScaledFrameBuffer();
	ScaledFrameBuffer(ViewPort& view);
	ScaledFrameBuffer(float widthPercent, float heightPercent);
	void resizeBuffer(int imgWidth, int imgHeight);
	void destroy();
};


class DrawFrameBuffer : public FrameBuffer {
public:
	DrawFrameBuffer();
};


void resizeFrameBuffers(int width, int height);