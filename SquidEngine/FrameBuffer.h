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
	void setClearColour(glm::vec3 clearColour);
	inline int getHeight() { return height; }
	inline int getWidth() { return width; }
	inline unsigned int getID() { return ID; };
	inline void destroy() {};
};

class StaticFrameBuffer : public FrameBuffer {
private:
	bool enableAntiAliasing;
protected:
	
	unsigned int colourBufferID;
	unsigned int renderBufferID;


	StaticFrameBuffer* nonSamplingBuffer;
	
	void initBuffer(int imgWidth, int imgHeight, glm::vec3& clearColour);
	void setAntiAliasing(bool state);

public:

	unsigned int getTextureOutput();

	StaticFrameBuffer();
	StaticFrameBuffer(int imgWidth, int imgHeight, bool doAntiAliasing);
	StaticFrameBuffer(int imgWidth, int imgHeight, bool doAntiAliasing, glm::vec3& clearColour);

	void resizeBuffer(int imgWidth, int imgHeight);
	void doAntiAliasing(bool state, unsigned int samples);
	void destroy();
};


class ScaledFrameBuffer : public StaticFrameBuffer {
private:
	float percentWidth, percentHeight;
public:
	static std::list<ScaledFrameBuffer*> scaledFrameBuffers;
	ScaledFrameBuffer();
	ScaledFrameBuffer(ViewPort& view, bool doAntiAliasing);
	ScaledFrameBuffer(float widthPercent, float heightPercent, bool doAntiAliasing);
	void resizeBuffer(int imgWidth, int imgHeight);
	void destroy();
};


class DrawFrameBuffer : public FrameBuffer {
public:
	DrawFrameBuffer();
	void updateScreenSize(int screenWidth, int screenHeight);
};



void resizeFrameBuffers(int width, int height);