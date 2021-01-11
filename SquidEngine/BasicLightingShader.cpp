#include "BasicLightingShader.h"
#include <iostream>
#include "GlobalConfig.h"
#include "ScreenQuad.h"

void BasicLightingShader::onInit() {
	
	newShaderPass();
	attachVertexShader("");
	attachFragmentShader("");
	finishShaderPass();
}


void BasicLightingShader::onNextPass(int shaderStage, unsigned int shaderID) {
	useLighting(shaderStage);
	useCamera(shaderStage,mainCamera);
	drawShapes(shaderStage);
}



void SkyboxLightingShader::onInit() {
	
	//Get shadow map
	std::cout << "Compile shadow map pass" << std::endl;
	newShaderPass();
	attachVertexShader("Shaders/ShadowMapShader/ShadowMapShader.vert");
	attachFragmentShader("Shaders/ShadowMapShader/ShadowMapShader.frag");
	finishShaderPass();
	
	//Get skybox reflection map
	std::cout << "Compile skybox reflection pass" << std::endl;
	newShaderPass();
	attachVertexShader("Shaders/SkyboxLightingShader/SkyboxReflectionShader.vert");
	attachFragmentShader("Shaders/SkyboxLightingShader/SkyboxReflectionShader.frag");
	finishShaderPass();
	
	//Draw Skybox
	std::cout << "Compile skybox pass" << std::endl;
	newShaderPass();
	attachVertexShader("Shaders/SkyboxLightingShader/SkyboxShader.vert");
	attachFragmentShader("Shaders/SkyboxLightingShader/SkyboxShader.frag");
	finishShaderPass();
	
	//Sample reflection map and apply lighting
	std::cout << "Compile main lighting pass" << std::endl;
	newShaderPass();
	attachVertexShader("Shaders/SkyboxLightingShader/SkyboxLightingShader.vert");
	attachFragmentShader("Shaders/SkyboxLightingShader/SkyboxLightingShader.frag");
	finishShaderPass();
	
	shadowMapBuffer = new StaticFrameBuffer(
		renderConfig.shadowWidth, renderConfig.shadowHeight, false, true, false);

	skyboxLightingBuffer = new StaticFrameBuffer(
		renderConfig.targetWidth, renderConfig.targetHeight,true,false,false);
}



void SkyboxLightingShader::onNextPass(int shaderStage, unsigned int shaderID) {
	
	useCamera(shaderStage, mainCamera);

	if (shaderStage == 0) {
		glCullFace(GL_FRONT);
		shadowMapBuffer->use();
		for (auto const& dLight : directionalLights) {
			Camera lightCam(glm::vec3(3, 7.0f, 0), glm::vec3(-0.25f, -0.25f, 0));
			lightCam.setView(renderConfig.shadowWidth, renderConfig.shadowHeight);
			lightCam.setClippingSize(30);
			lightCam.setPerspective(CAMERA_ORTHOGRAPHIC);
			lightCam.setRenderDistance(1.0f, 30.5f);
			useCamera(shaderStage, &lightCam);
			drawShapes(shaderStage);
			break;
		}

		glBindTexture(GL_TEXTURE_2D,shadowMapBuffer->getDepthOutput());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		glBindTexture(GL_TEXTURE_2D, 0);
		targetBuffer->use();
		glCullFace(GL_NONE);
	}
	

	if (shaderStage == 1) {
		
		skyboxLightingBuffer->use();
		glActiveTexture(GL_TEXTURE20); // Safe number to avoid overwriting material textures
		glBindTexture(GL_TEXTURE_CUBE_MAP, sceneSkyBox->getTextureID());
		drawShapes(shaderStage);
		targetBuffer->use();
	}

	if (shaderStage == 2) {
		glDepthFunc(GL_LEQUAL);
		glm::mat4 viewMatrixNoTranslation = glm::mat4(glm::mat3(mainCamera->getViewMatrix()));
		imageMatricesUBO->setData(0,sizeof(glm::mat4),&viewMatrixNoTranslation);
		setMat4(shaderStage, MODEL_MATRIX_UNIFORM, sceneSkyBox->getModelMatrix(), -1);
		sceneSkyBox->draw();
		glDepthFunc(GL_LESS);
	}

	if (shaderStage == 3) {
		glActiveTexture(GL_TEXTURE20); // Safe number to avoid overwriting material textures
		glBindTexture(GL_TEXTURE_2D, skyboxLightingBuffer->getTextureOutput());
		int loc = glGetUniformLocation(shaderID, getIndexedUniform("skyboxReflections", -1).c_str());
		glUniform1i(loc, 20);

		glActiveTexture(GL_TEXTURE21); // Safe number to avoid overwriting material textures
		glBindTexture(GL_TEXTURE_2D, shadowMapBuffer->getDepthOutput());
		loc = glGetUniformLocation(shaderID, getIndexedUniform("shadowMap", -1).c_str());
		glUniform1i(loc, 21);


		Camera lightCam(glm::vec3(3, 7.0f, 0), glm::vec3(-0.25f, -0.25f, 0));
		lightCam.setView(renderConfig.shadowWidth, renderConfig.shadowHeight);
		lightCam.setClippingSize(30);
		lightCam.setPerspective(CAMERA_ORTHOGRAPHIC);
		lightCam.setRenderDistance(1.0f, 30.5f);

		setMat4(shaderStage, "lightView",lightCam.getViewMatrix(),-1);
		setMat4(shaderStage, "lightProj", lightCam.getProjectionMatrix(), -1);

		useLighting(shaderStage);
		drawShapes(shaderStage);
	}

}

void SkyboxLightingShader::destroy() {
	std::cout << "Lighting Shader destroyed" << std::endl;
	skyboxLightingBuffer->destroy();
	shadowMapBuffer->destroy();
	delete skyboxLightingBuffer;
	delete shadowMapBuffer;
	ShaderProgram2::destroy();
}





