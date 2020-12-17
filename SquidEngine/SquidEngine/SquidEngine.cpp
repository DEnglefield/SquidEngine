

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


bool windowFocused = false;
CameraFPS defaultCamera(0, 0, 3, 0, 0, -1);
Window mainWindow(800, 600, "SquidEngine");
EngineInstance* currentInstance;



std::vector<ViewPort> viewPorts = { ViewPort(0.0f, 0.0f, 1.0f, 1.0f) };


void mainResizeEvent(GLFWwindow* window, int width, int height) {
	resizeFrameBuffers(width, height);
	currentInstance->onWindowResize(width, height);
}



void cursorCallBack(GLFWwindow* window, double xpos, double ypos) {
	currentInstance->onMouseMove(xpos, ypos);
	if (windowFocused) {
		defaultCamera.updateCursorPos(xpos, ypos, true);
	}
	else {
		defaultCamera.updateCursorPos(xpos, ypos, false);
	}
	
	
}

void scrollCallBack(GLFWwindow* window, double scrollX, double scrollY) {

	if (defaultCamera.getMoveSpeed() + scrollY < 0) {
		defaultCamera.setMoveSpeed(0);
	}
	else {
		defaultCamera.setMoveSpeed(defaultCamera.getMoveSpeed() + scrollY) ;
	}
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

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	currentInstance->onInit();

	//ScaledFrameBuffer targetBuffer(1, 1);
	
	DrawFrameBuffer outputBuffer;
	//ScreenQuad quad(1,1,1,1);

	//targetBuffer.setClearColour(glm::vec3(0.75f, 0.25f, 0.25f));
	

	double frameStart = glfwGetTime();
	double frameTime = 0;
	while (!mainWindow.closing()) {

		currentInstance->onInputCheck(mainWindow.form);

		for (int i = 0; i < viewPorts.size(); ++i) {
			viewPorts[i].use();
			currentInstance->onDraw(frameTime, i, viewPorts[i]);

			for (auto const& shader : ShaderProgram2::sceneShaders) {									
				shader->draw(outputBuffer);
			}
			
		}


		glfwSwapInterval(1);
		glfwSwapBuffers(glfwGetCurrentContext());
		glfwPollEvents();

		frameTime = glfwGetTime() - frameStart;
		defaultCamera.updateFPS(1 / frameTime);
		frameStart = glfwGetTime();
	}	

	currentInstance->onClose();
	glfwTerminate();
}

