
#include <iostream>
#include "FrameBuffer.h"
#include "Texture.h"
#include "GlobalConfig.h"

std::list<ScaledFrameBuffer*> ScaledFrameBuffer::scaledFrameBuffers;

ScaledFrameBuffer::ScaledFrameBuffer() : StaticFrameBuffer() {};

ScaledFrameBuffer::ScaledFrameBuffer(ViewPort& view, bool addColour, 
	bool addDepthStencil, bool doAntiAliasing)
	: StaticFrameBuffer(view.width, view.height, addColour, addDepthStencil, doAntiAliasing) {

	scaledFrameBuffers.push_back(this);
	percentWidth = view.windowWidthPercent;
	percentHeight = view.windowHeightPercent;
}


ScaledFrameBuffer::ScaledFrameBuffer(float widthPercent, float heightPercent, 
	bool addColour, bool addDepthStencil, bool doAntiAliasing) 
	: StaticFrameBuffer(widthPercent * 800,heightPercent * 600, addColour, 
		addDepthStencil, doAntiAliasing) {

	scaledFrameBuffers.push_back(this);
	percentWidth = widthPercent;
	percentHeight = heightPercent;
}


void ScaledFrameBuffer::destroy() {
	scaledFrameBuffers.remove(this);
	StaticFrameBuffer::destroy();
}


void ScaledFrameBuffer::resizeBuffer(int imgWidth, int imgHeight) {
	StaticFrameBuffer::resizeBuffer(1 * imgWidth, 1 * imgHeight);
}


StaticFrameBuffer::StaticFrameBuffer() : FrameBuffer() {}

StaticFrameBuffer::StaticFrameBuffer(int imgWidth, int imgHeight, bool addColour, 
	bool addDepthStencil, bool doAntiAliasing) : FrameBuffer() {

	clearCol = glm::vec3(0.43f, 0.71f, 0.86f);
	glGenFramebuffers(1, &ID);
	setAntiAliasing(doAntiAliasing);
	nonSamplingBuffer = nullptr;
	hasDepthAttachment = addDepthStencil;
	hasColourAttachment = addColour;
	initBuffer(imgWidth, imgHeight, clearCol);
}


StaticFrameBuffer::StaticFrameBuffer(int imgWidth, int imgHeight, bool addColour, 
	bool addDepthStencil, bool doAntiAliasing, glm::vec3& clearColour)
	: FrameBuffer() {

	glGenFramebuffers(1, &ID);
	setAntiAliasing(doAntiAliasing);
	nonSamplingBuffer = nullptr;
	hasDepthAttachment = addDepthStencil;
	hasColourAttachment = addColour;
	initBuffer(imgWidth, imgHeight, clearColour);
}


void StaticFrameBuffer::doAntiAliasing(bool state, unsigned int samples) {
	bool aaEnabled = enableAntiAliasing;
	setAntiAliasing(state);
	if (enableAntiAliasing != aaEnabled) {
		resizeBuffer(width, height);
	}
}


void StaticFrameBuffer::setAntiAliasing(bool state) {
	if (state && renderConfig.enableMSAA) {
		enableAntiAliasing = true;
	}
	else {
		enableAntiAliasing = false;
	}
}


void StaticFrameBuffer::initBuffer(int imgWidth, int imgHeight, glm::vec3& clearColour) {

	width = imgWidth;
	height = imgHeight;
	
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	if (hasColourAttachment) { attachColour(width,height,clearColour); }
	if (hasDepthAttachment) { attachDepthStencil(width, height); }
	else { attachDepthStencilRenderBuffer(width, height); }

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer: " << ID << " is not complete!" << std::endl;
	}
	else {
		std::cout << "Frame buffer: "<< ID << " complete" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void StaticFrameBuffer::attachColour(int imgWidth, int imgHeight, glm::vec3& clearColour) {
	setClearColour(clearColour);

	if (enableAntiAliasing) {
		if (nonSamplingBuffer == nullptr) {
			nonSamplingBuffer = new StaticFrameBuffer(imgWidth, imgHeight, true, hasDepthAttachment, false);
		}
		else {
			nonSamplingBuffer->resizeBuffer(imgWidth, imgHeight);
		}
	}
	else if (nonSamplingBuffer != nullptr) {
		nonSamplingBuffer->destroy();
		delete nonSamplingBuffer;
		nonSamplingBuffer = nullptr;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, ID);

	glGenTextures(1, &colourBufferID);

	if (enableAntiAliasing) {
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, colourBufferID);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, renderConfig.numSmaplesMSAA
			, GL_RGBA, imgWidth, imgHeight, GL_TRUE);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, colourBufferID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 
			0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	
	if (enableAntiAliasing) {
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, 
			colourBufferID, 0);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 
			colourBufferID, 0);
	}
}


void StaticFrameBuffer::attachDepthStencil(int imgWidth, int imgHeight) {

	glGenTextures(1, &depthStencilBufferID);
	
	if (enableAntiAliasing) {
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, depthStencilBufferID);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, renderConfig.numSmaplesMSAA, 
			GL_DEPTH24_STENCIL8, imgWidth, imgHeight, GL_TRUE);
		}

	else {
		glBindTexture(GL_TEXTURE_2D, depthStencilBufferID);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, imgWidth, imgHeight, 0,
			GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if (enableAntiAliasing) {
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, depthStencilBufferID, 0);
	}else{
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthStencilBufferID, 0);
	}
}


void StaticFrameBuffer::attachDepthStencilRenderBuffer(int imgWidth, int imgHeight) {
	
	glGenRenderbuffers(1, &renderBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferID);
	if (enableAntiAliasing) {
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, renderConfig.numSmaplesMSAA
			, GL_DEPTH24_STENCIL8, imgWidth, imgHeight);
	}
	else {
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, imgWidth, imgHeight);
	}
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferID);
}



void StaticFrameBuffer::resizeBuffer(int imgWidth, int imgHeight) {
	if (hasColourAttachment) { glDeleteTextures(1, &colourBufferID); }
	if (hasDepthAttachment) { glDeleteTextures(1, &depthStencilBufferID); }
	else { glDeleteRenderbuffers(1, &renderBufferID); }

	initBuffer(imgWidth, imgHeight, clearCol);

	std::cout << "Resized framebuffer: " << ID << " to: " << width << ", " << height << std::endl;
}


void StaticFrameBuffer::destroy() {

	if (hasColourAttachment) { 
		if (enableAntiAliasing) {
			nonSamplingBuffer->destroy();
			delete nonSamplingBuffer;
		}
		glDeleteTextures(1, &colourBufferID); 
	}

	if (hasDepthAttachment) { glDeleteTextures(1, &depthStencilBufferID); }
	else { glDeleteRenderbuffers(1, &renderBufferID); }

	glDeleteFramebuffers(1, &ID);
	FrameBuffer::destroy();
}



unsigned int StaticFrameBuffer::getTextureOutput() {
	if (enableAntiAliasing) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, ID);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, nonSamplingBuffer->getID());

		glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, 
			GL_COLOR_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		return nonSamplingBuffer->getTextureOutput();
	}
	return colourBufferID; 
}


unsigned int StaticFrameBuffer::getDepthOutput() {
	if (enableAntiAliasing) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, ID);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, nonSamplingBuffer->getID());

		glBlitFramebuffer(0, 0, width, height, 0, 0, width, height,
			GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return nonSamplingBuffer->getTextureOutput();
	}	
	return depthStencilBufferID;
}

void FrameBuffer::setClearColour(glm::vec3 clearColour) { clearCol = clearColour; }

void FrameBuffer::use() {

	if (ID == 0) {
		//std::cout << width << ", " << height << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	glViewport(0,0,width,height);
	glClearColor(clearCol.x, clearCol.y, clearCol.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //GL_STENCIL_BUFFER_BIT
}


DrawFrameBuffer::DrawFrameBuffer() {
	setClearColour(glm::vec3(0.43f, 0.71f, 0.86f));
	ID = 0;
}


void DrawFrameBuffer::updateScreenSize(int screenWidth, int screenHeight) {
	width = screenWidth;
	height = screenHeight;
}



void resizeFrameBuffers(int width, int height) {
	std::cout << "Framebuffer resize all: " << width << ", " << height << std::endl;
	for (auto const& scaledBuffer : ScaledFrameBuffer::scaledFrameBuffers) {
		scaledBuffer->resizeBuffer(width, height);
	}

}

