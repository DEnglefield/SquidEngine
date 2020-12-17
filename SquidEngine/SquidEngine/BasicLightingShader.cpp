#include "BasicLightingShader.h"
#include <iostream>

void BasicLightingShader::onInit() {
	
	
	newShaderPass(
		"Shaders/LightingShader/LightingShader.vert",
		"Shaders/LightingShader/LightingShader.frag", 
		nullptr);
	

	/*
	newShaderPass(
		"Shaders/BasicShader/BasicShader.vert",
		"Shaders/BasicShader/BasicShader.frag",
		nullptr);
	*/

	finishShaderPass();

	glUseProgram(shaderPasses[0].shaderID);
	setMat4(0, VIEW_MATRIX_UNIFORM, glm::mat4(1.0f), -1);
	setMat4(0, PROJECTION_MATRIX_UNIFORM, glm::mat4(1.0f), -1);
}


void BasicLightingShader::onNextPass(int shaderStage, unsigned int shaderID) {
	useLighting(shaderStage);
	useCamera(shaderStage,mainCamera);
	drawShapes(shaderStage);
}
