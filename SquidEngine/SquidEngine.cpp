

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

//GLFW Resize event
void mainResizeEvent(GLFWwindow* window, int width, int height) {
	std::cout << "Resize event: " << width << ", " << height << std::endl;
	//Resize framebuffers
	screenFrameBuffer->updateScreenSize(width, height);
	resizeFrameBuffers(width, height);
	currentInstance->onWindowResize(width, height);
}


//GLFW Cursor move event
void cursorCallBack(GLFWwindow* window, double xpos, double ypos) {
	currentInstance->onMouseMove(xpos, ypos);
}

//GLFW Scroll event
void scrollCallBack(GLFWwindow* window, double scrollX, double scrollY) {

	currentInstance->onMouseScroll(scrollX, scrollY);
}

//GLFW Mouse button press event
void mouseBtnCallBack(GLFWwindow* window, int button, int action, int mods) {
	if (glfwGetWindowAttrib(window, GLFW_HOVERED)) {
		if (button == GLFW_MOUSE_BUTTON_1) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			windowFocused = true;
		}
	}
	currentInstance->onMouseButton(button, action);
}


//GLFW key press event
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	
	if (key == GLFW_KEY_ESCAPE) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		windowFocused = false;
	}
}


//Get if the window is currently active on the host OS
bool isWindowFocused() { return windowFocused; }


//Initialize the application window with default options
void createEngineWindow(int width, int height, const char* title) {
	mainWindow = Window(width,height,title);
}


//Update the list of screen partitionst to apply to the output framebuffer
void setViewPorts(ViewPort* views, int numViews) {
	viewPorts.clear();
	viewPorts.insert(viewPorts.begin(), views, views+numViews);
}


//Open the window and initialize OpenGL context beginning render loop
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

	//Create openGL context with glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_SCISSOR_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Screen output framebuffer (ID 0)
	screenFrameBuffer = new DrawFrameBuffer;

	//FrameBuffer used to hold rendered geometry
	StaticFrameBuffer renderFrameBuffer(renderConfig.targetWidth
		,renderConfig.targetHeight, true, true, false);

	//Post processing shader with common operations (gamma correction)
	CommonPostShader postProcessShader;

	//Perform resize event to initialize framebuffer sizes
	int winWidth, winHeight;
	mainWindow.getWindowSize(&winWidth, &winHeight);
	mainResizeEvent(mainWindow.form, winWidth, winHeight);

	postProcessShader.onInit();

	//Trigger app instance initalization event
	currentInstance->onInit();

	double frameStart = glfwGetTime();
	double frameTime = 0;
	//Begin main render loop
	while (!mainWindow.closing()) {
		glfwPollEvents();
		currentInstance->onInputCheck(mainWindow.form);

		//Perform a draw call for each screen partition
		for (int i = 0; i < viewPorts.size(); ++i) {
			
			viewPorts[i].use();
			//Trigger app instance on draw event
			currentInstance->onDraw(frameTime, i, viewPorts[i]);

			//Draw to renderFrameBuffer using each active shader and its bound geometry
			for (auto const& shader : ShaderProgram2::sceneShaders) {
				if (shader->isEnabled()) {
					shader->draw(renderFrameBuffer);
				}	
			}
		}

		//Setup and run post processing shader
		postProcessShader.setFloat(0,"gamma",renderConfig.gamma,-1);
		postProcessShader.setInputRender(renderFrameBuffer.getTextureOutput()); 
		postProcessShader.draw(*screenFrameBuffer);

		glfwSwapInterval(renderConfig.enableVSync);
		glfwSwapBuffers(glfwGetCurrentContext());
		
		//Track current FPS
		frameTime = glfwGetTime() - frameStart;
		while (frameTime < 1.0f/renderConfig.targetFPS) { frameTime = glfwGetTime() - frameStart; }
		frameStart = glfwGetTime();

		//std::cout << "FPS: " << 1 / frameTime << std::endl;
	}	

	postProcessShader.destroy();
	//Trigger app instance on close event
	currentInstance->onClose();
	glfwTerminate();
}

