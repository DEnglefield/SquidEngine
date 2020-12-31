#pragma once

#include "ShaderProgram2.h"
#include "CubeMap.h"

class BasicLightingShader: public ShaderProgram2 {
private:
	void onNextPass(int shaderStage, unsigned int shaderID) override;
public:
	void onInit() override;
};

class SkyboxLightingShader : public ShaderProgram2 {
private:
	SkyBox* sceneSkyBox;
	ScaledFrameBuffer* skyboxLightingBuffer;
	void onNextPass(int shaderStage, unsigned int shaderID) override;
public:
	void onInit() override;
	void destroy();
	inline void setSkybox(SkyBox& skybox) { sceneSkyBox = &skybox; };
};