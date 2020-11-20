
#include "FrameBuffer.h"

std::vector<FrameBuffer*> FrameBuffer::scaledFrameBuffers;

FrameBuffer::FrameBuffer(GLFWwindow* window) {
	clearCol = glm::vec3(0.43f, 0.71f, 0.86f);
	scaledFrameBuffers.push_back(this); //need a way to remove it
	glGenFramebuffers(1, &ID);
	glfwGetWindowSize(window, &width, &height);
	initBuffer(width, height, clearCol);
}


FrameBuffer::FrameBuffer(int imgWidth, int imgHeight) {
	clearCol = glm::vec3(0.43f, 0.71f, 0.86f);
	glGenFramebuffers(1, &ID);
	initBuffer(imgWidth, imgHeight, clearCol);
}


FrameBuffer::FrameBuffer(int imgWidth, int imgHeight, glm::vec3& clearColour) {
	glGenFramebuffers(1, &ID);
	initBuffer(imgWidth, imgHeight, clearColour);
}


void FrameBuffer::initBuffer(int imgWidth, int imgHeight, glm::vec3& clearColour) {
	setClearColour(clearColour);

	width = imgWidth;
	height = imgHeight;

	glBindFramebuffer(GL_FRAMEBUFFER, ID);

	Texture colourBuffer(width, height);
	
	colourBuffer.setScaleUpFilter(GL_LINEAR);
	colourBuffer.setScaleDownFilter(GL_LINEAR);
	colourBuffer.setWrappingX(GL_CLAMP_TO_BORDER);
	colourBuffer.setWrappingY(GL_CLAMP_TO_BORDER);
	colourBufferID = colourBuffer.getID();
	std::cout << "Resized texture: " << colourBuffer.getID() << " to: " << width << ", " << height << std::endl;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colourBufferID, 0);

	glGenRenderbuffers(1, &renderBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferID);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void FrameBuffer::resizeBuffer(int imgWidth, int imgHeight) {
	glDeleteTextures(1, &colourBufferID);
	glDeleteRenderbuffers(1, &renderBufferID);
	initBuffer(imgWidth, imgHeight, clearCol);
	std::cout << "Resized framebuffer: " << ID << " to: " << width << ", " << height << std::endl;
}


void FrameBuffer::destroy() { 
	glDeleteTextures(1, &colourBufferID);
	glDeleteRenderbuffers(1, &renderBufferID);
	glDeleteFramebuffers(1, &ID);

}


void FrameBuffer::setClearColour(glm::vec3 clearColour) {clearCol = clearColour;}

unsigned int FrameBuffer::getTextureOutput() { return colourBufferID; }

void FrameBuffer::use() {
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	glViewport(0, 0, width, height);
	glClearColor(clearCol.x, clearCol.y, clearCol.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


void FrameBuffer::use(struct ViewPort& viewport) {
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	viewport.use();
	//glViewport(viewport.xPos, viewport.yPos, viewport.width, viewport.height);
	glClearColor(clearCol.x, clearCol.y, clearCol.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


int FrameBuffer::getWidth() { return width; }
int FrameBuffer::getHeight() { return height; };


DrawFrameBuffer::DrawFrameBuffer() { setClearColour(glm::vec3(0.43f, 0.71f, 0.86f)); }

void DrawFrameBuffer::setClearColour(glm::vec3 clearColour) { clearCol = clearColour; }

void DrawFrameBuffer::use() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	int winWidth, winHeight;
	glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
	glViewport(0, 0, winWidth, winHeight);
	glClearColor(clearCol.x, clearCol.y, clearCol.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


void DrawFrameBuffer::use(struct ViewPort& viewport) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	viewport.use();
	glClearColor(clearCol.x, clearCol.y, clearCol.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


int DrawFrameBuffer::getWidth() { return 0; }
int DrawFrameBuffer::getHeight() { return 0; };

void resizeFrameBuffers(int width, int height) {
	for (int i = 0; i < FrameBuffer::scaledFrameBuffers.size(); ++i) {
		FrameBuffer::scaledFrameBuffers[i]->resizeBuffer(width, height);
	}
}

