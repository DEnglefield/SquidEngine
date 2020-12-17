#pragma once

#include "Window.h"

class EngineInstance {
public:
	virtual void onInit() = 0;
	virtual void onDraw(double deltaTime, int viewID, ViewPort& view) = 0;
	virtual void onClose() = 0;

	virtual void onWindowResize(int width, int height) = 0;
	virtual void onMouseScroll(double scrollX, double scrollY) = 0;
	virtual void onMouseMove(int mouseX, int mouseY) = 0;
	virtual void onMouseButton(int buttonID, int action) = 0;
	virtual void onInputCheck(GLFWwindow* window) =0;
};