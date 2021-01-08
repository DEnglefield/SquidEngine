#pragma once

#include <glad\glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <list>
#include "Window.h"

//Base framebuffer class used to implement a common interface
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


//A framebuffer with a fixed size 
//Can hold any combination of colour, and depth attachments
class StaticFrameBuffer : public FrameBuffer {
private:
	bool enableAntiAliasing;
protected:
	
	unsigned int colourBufferID;
	unsigned int depthStencilBufferID;  
	unsigned int renderBufferID;

	bool hasDepthAttachment;
	bool hasColourAttachment;

	StaticFrameBuffer* nonSamplingBuffer;
	
	void initBuffer(int imgWidth, int imgHeight, glm::vec3& clearColour);
	void setAntiAliasing(bool state);

	void attachColour(int imgWidth, int imgHeight, glm::vec3& clearColour);
	void attachDepthStencil(int imgWidth, int imgHeight);
	void attachDepthStencilRenderBuffer(int imgWidth, int imgHeight);

public:

	unsigned int getTextureOutput();
	unsigned int getDepthOutput();

	StaticFrameBuffer();
	StaticFrameBuffer(int imgWidth, int imgHeight, bool addColour, bool addDepthStencil, bool doAntiAliasing);
	StaticFrameBuffer(int imgWidth, int imgHeight, bool addColour, bool addDepthStencil, bool doAntiAliasing, glm::vec3& clearColour);

	void resizeBuffer(int imgWidth, int imgHeight);
	void doAntiAliasing(bool state, unsigned int samples);
	void destroy();
};


//A framebuffer that is scaled to be proportional with the output framebuffer
class ScaledFrameBuffer : public StaticFrameBuffer {
private:
	float percentWidth, percentHeight;
public:
	static std::list<ScaledFrameBuffer*> scaledFrameBuffers;
	ScaledFrameBuffer();
	ScaledFrameBuffer(ViewPort& view, bool addColour, bool addDepthStencil, bool doAntiAliasing);
	ScaledFrameBuffer(float widthPercent, float heightPercent, bool addColour, bool addDepthStencil, bool doAntiAliasing);
	void resizeBuffer(int imgWidth, int imgHeight);
	void destroy();
};


//A framebuffer with the ID locked at 0 representing the screen framebuffer
class DrawFrameBuffer : public FrameBuffer {
public:
	DrawFrameBuffer();
	void updateScreenSize(int screenWidth, int screenHeight);
};


//Update the DrawFrameBuffer with current window size and resize all ScaledFrameBuffer instances
void resizeFrameBuffers(int width, int height);