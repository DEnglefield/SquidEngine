

#include <glad\glad.h>
#include <iostream>
#include <vector>
#include "SquidEngine.h"

#include "FrameBuffer.h"
#include "ScreenQuad.h"
#include "ShaderProgram2.h"

#include "CameraFPS.h"
#include "Materials.h"
#include "Shape.h"
#include "CommonPostShader.h"
#include "GlobalConfig.h"
#include "stb_image_write.h"


bool windowFocused = false;
Window mainWindow(800, 600, "SquidEngine");
EngineInstance* currentInstance;
DrawFrameBuffer* screenFrameBuffer;


std::vector<ViewPort> viewPorts = { ViewPort(0.0f, 0.0f, 1.0f, 1.0f) };


void mainResizeEvent(GLFWwindow* window, int width, int height) {
	std::cout << "Resize event: " << width << ", " << height << std::endl;
	screenFrameBuffer->updateScreenSize(width, height);
	resizeFrameBuffers(width, height);
	currentInstance->onWindowResize(width, height);
}



void cursorCallBack(GLFWwindow* window, double xpos, double ypos) {
	currentInstance->onMouseMove(xpos, ypos);
}

void scrollCallBack(GLFWwindow* window, double scrollX, double scrollY) {

	currentInstance->onMouseScroll(scrollX, scrollY);
}

void mouseBtnCallBack(GLFWwindow* window, int button, int action, int mods) {
	if (glfwGetWindowAttrib(window, GLFW_HOVERED)) {
		if (button == GLFW_MOUSE_BUTTON_1) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			windowFocused = true;
		}
	}
	currentInstance->onMouseButton(button, action);
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	
	if (key == GLFW_KEY_ESCAPE) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		windowFocused = false;
	}
}


bool isWindowFocused() { return windowFocused; }


void createEngineWindow(int width, int height, const char* title) {
	mainWindow = Window(width,height,title);
}


void setViewPorts(ViewPort* views, int numViews) {
	viewPorts.clear();
	viewPorts.insert(viewPorts.begin(), views, views+numViews);
}


void runEngine(EngineInstance& instance) {

	currentInstance = &instance;

	//glfwWindowHint(GLFW_SAMPLES, 4); //Not needed unless rendering geometry to ouput framebuffer

	mainWindow.show();
	glfwSetFramebufferSizeCallback(mainWindow.form, mainResizeEvent);
	glfwSetCursorPosCallback(mainWindow.form, cursorCallBack);
	glfwSetMouseButtonCallback(mainWindow.form, mouseBtnCallBack);
	glfwSetScrollCallback(mainWindow.form, scrollCallBack);
	glfwSetKeyCallback(mainWindow.form, key_callback);

	mainWindow.setActive();

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_SCISSOR_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	screenFrameBuffer = new DrawFrameBuffer;

	StaticFrameBuffer renderFrameBuffer(renderConfig.targetWidth
		,renderConfig.targetHeight ,true);

	CommonPostShader postProcessShader;

	int winWidth, winHeight;
	mainWindow.getWindowSize(&winWidth, &winHeight);
	mainResizeEvent(mainWindow.form, winWidth, winHeight);

	postProcessShader.onInit();

	currentInstance->onInit();

	Texture test("Resources/Textures/star.png",TEXTURE_DIFFUSE_MAP);

	double frameStart = glfwGetTime();
	double frameTime = 0;
	while (!mainWindow.closing()) {
		glfwPollEvents();
		currentInstance->onInputCheck(mainWindow.form);

		for (int i = 0; i < viewPorts.size(); ++i) {
			
			viewPorts[i].use();
			currentInstance->onDraw(frameTime, i, viewPorts[i]);
			for (auto const& shader : ShaderProgram2::sceneShaders) {
				if (shader->isEnabled()) {
					shader->draw(renderFrameBuffer);
				}	
			}
		}

		//postProcessShader.setInputRender(test.getID()); //1920, 1080
		//postProcessShader.draw(renderFrameBuffer);
		
		/*
		unsigned char* imageData = new unsigned char[4*(renderFrameBuffer.getWidth() * renderFrameBuffer.getHeight())];
		glBindTexture(GL_TEXTURE_2D, renderFrameBuffer.getTextureOutput());
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		stbi_write_jpg("test.jpg", renderFrameBuffer.getWidth(), renderFrameBuffer.getHeight(), 4, imageData,100);
		break;
		*/
		postProcessShader.setFloat(0,"gamma",renderConfig.gamma,-1);
		postProcessShader.setInputRender(renderFrameBuffer.getTextureOutput()); //800,600
		postProcessShader.draw(*screenFrameBuffer);

		glfwSwapInterval(0);
		glfwSwapBuffers(glfwGetCurrentContext());
		

		frameTime = glfwGetTime() - frameStart;
		while (frameTime < 1.0f/renderConfig.targetFPS) { frameTime = glfwGetTime() - frameStart; }
		frameStart = glfwGetTime();

		//std::cout << "FPS: " << 1 / frameTime << std::endl;
	}	

	//postProcessShader.destroy();
	currentInstance->onClose();
	glfwTerminate();
}

