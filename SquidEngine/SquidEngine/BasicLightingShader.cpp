#include "BasicLightingShader.h"
#include <iostream>

void BasicLightingShader::onInit() {
	
	newShaderPass(
		"Shaders/LightingShader/LightingShader.vert",
		"Shaders/LightingShader/LightingShader.frag");

	finishShaderPass();
}


void BasicLightingShader::onNextPass(int shaderStage, unsigned int shaderID) {
	useLighting(shaderStage);
	useCamera(shaderStage,mainCamera);
	drawShapes(shaderStage);
}



void SkyboxLightingShader::onInit() {
	newShaderPass(
		"Shaders/SkyboxLightingShader/SkyboxLightingShader.vert",
		"Shaders/SkyboxLightingShader/SkyboxLightingShader.frag");

	finishShaderPass();
	
	newShaderPass(
		"Shaders/SkyboxLightingShader/SkyboxShader.vert",
		"Shaders/SkyboxLightingShader/SkyboxShader.frag");

	finishShaderPass();
	
}



void SkyboxLightingShader::onNextPass(int shaderStage, unsigned int shaderID) {
	glActiveTexture(GL_TEXTURE2); // Safe number to avoid overwriting material textures
	glBindTexture(GL_TEXTURE_CUBE_MAP, sceneSkyBox->getTextureID());
	useCamera(shaderStage, mainCamera);

	if (shaderStage == 0) {
		useLighting(shaderStage);
		drawShapes(shaderStage);
	}

	if (shaderStage == 1) {
		glDepthFunc(GL_LEQUAL);
		glm::mat4 viewMatrixNoTranslation = glm::mat4(glm::mat3(mainCamera->getViewMatrix()));
		setMat4(shaderStage, VIEW_MATRIX_UNIFORM, viewMatrixNoTranslation, -1);
		setMat4(shaderStage, MODEL_MATRIX_UNIFORM, sceneSkyBox->getModelMatrix(), -1);
		sceneSkyBox->draw();
		glDepthFunc(GL_LESS);
	}
	
}





