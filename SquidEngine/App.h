#pragma once

#include "EngineInstance.h"
#include "SquidEngine.h"
#include "Cube.h"
#include "CameraFPS.h"
#include "BasicLightingShader.h"
#include "Model.h"

//Game instance with interface to game engine
class Game : public EngineInstance {
public:
	//Called on engine initialisation
	void onInit() override;

	//Called on each draw call
	void onDraw(double deltaTime, int viewID, ViewPort& view) override;

	//Called on input check event
	void onClose() override;
	
	//Called on window and framebuffer resize
	void onWindowResize(int width, int height);
	//Called on scroll wheel moved
	void onMouseScroll(double scrollX, double scrollY);
	//Called on mosue move event
	void onMouseMove(int mouseX, int mouseY);
	//Called on mouse button click
	void onMouseButton(int buttonID, int action) {}
	//Called on key press event
	void onInputCheck(GLFWwindow* window);
};