
#include "Window.h"
#include <iostream>


//Flag to track state of GLFW
bool glfwInitialized = false;
int activeWindows = 0;

//Pointer to currently active viewport
ViewPort* activeViewPort;

//Create the window object and initialize if needed
Window::Window(int width, int height, const char* name) {
	if (!glfwInitialized) {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwInitialized = true;
	}
	windowWidth = width;
	windowHeight = height;
	windowName = name;
	++activeWindows;
}

//Destory the window state and terminate GLFW is last window
Window::~Window() {
	--activeWindows;
	if (activeWindows == 0) { 
		glfwTerminate(); 
		glfwInitialized = false;
	}
}

//Create and show the window
void Window::show() {
	form = glfwCreateWindow(windowWidth, windowHeight, windowName, NULL, NULL);
	if (form == NULL) { std::cout << "Failed to create GLFW window" << std::endl; }
}

//Get the state of the given key
//Returns GLFW_PRESS, GLFW_RELEASE, GLFW_REPEAT
int Window::getKey(int key) { return glfwGetKey(form, key); }
//Send request to close the window
void Window::close() { glfwSetWindowShouldClose(form, true); }
//Set this window as the current context
//All future draw command will be directed to this window
void Window::setActive() { glfwMakeContextCurrent(form); }
//Check if there is an active request to close the window
bool Window::closing() { return glfwWindowShouldClose(form); }
//Swap buffers in swap chain for this window
void Window::swapBuffers() { glfwSwapBuffers(form); }
//Update the width and height values with the windows current values
void Window::getWindowSize(int* width, int* height) { glfwGetWindowSize(form, width, height); }


//Create the viewport area using percentage positions on the window
ViewPort::ViewPort(float xPercent, float yPercent, float widthPercent, float heightPercent) {
	windowPercentX = xPercent;
	windowPercentY = yPercent;
	windowWidthPercent = widthPercent;
	windowHeightPercent = heightPercent;
}


//Set this viewport as the active viewport meaning all future draw commands
//will be directed to this viewport
//Also scales the viewport with the active window
void ViewPort::use() {
	int winWidth, winHeight;
	glfwGetWindowSize(glfwGetCurrentContext(), &winWidth, &winHeight);
	
	xPos = winWidth * windowPercentX;
	yPos = winHeight * windowPercentY;
	width = winWidth * windowWidthPercent;
	height = winHeight * windowHeightPercent;

	glViewport(xPos, yPos, width, height);
	glScissor(xPos, yPos, width, height);
	activeViewPort = this;
}

//Get the current active viewport
ViewPort& getCurrentViewPort() {
	return *activeViewPort;
}


