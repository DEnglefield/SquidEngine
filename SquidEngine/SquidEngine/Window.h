#pragma once

#include "SquidEngine.h"

//Window object acting as a container for viewports
class Window {
protected:
	int windowWidth, windowHeight;
	const char* windowName;
public:
	//Wrapped glfw window
	GLFWwindow* form;
	Window(int width, int height, const char* name);
	~Window();
	//Create and show the window
	void show();
	//Send request to close the window
	void close();
	//Set this window as the current context
	//All future draw command will be directed to this window
	void setActive();
	//Check if there is an active request to close the window
	bool closing();
	//Get the state of the given key
	//Returns GLFW_PRESS, GLFW_RELEASE, GLFW_REPEAT
	int getKey(int key);
	//Swap buffers in swap chain for this window
	void swapBuffers();
	//Updates the given values with the window's current width and height
	void getWindowSize(int* width, int* height);

	//Subscribe to a resize event with the given function
	void setResizeEvent(void* resizeFunc);
};