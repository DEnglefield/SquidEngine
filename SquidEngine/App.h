#pragma once

#include "EngineInstance.h"
#include "SquidEngine.h"
#include "Cube.h"
#include "CameraFPS.h"
#include "BasicLightingShader.h"
#include "Model.h"

class Game : public EngineInstance {
public:
	void onInit() override;
	void onDraw(double deltaTime, int viewID, ViewPort& view) override;
	void onClose() override;
	
	void onWindowResize(int width, int height);
	void onMouseScroll(double scrollX, double scrollY);
	void onMouseMove(int mouseX, int mouseY);
	void onMouseButton(int buttonID, int action) {}
	void onInputCheck(GLFWwindow* window);
};