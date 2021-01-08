#pragma once

#include "ShaderProgram2.h"
#include "CubeMap.h"

//Lighting shader to perform blinn-phong shading only
class BasicLightingShader: public ShaderProgram2 {
private:
	void onNextPass(int shaderStage, unsigned int shaderID) override;
public:
	void onInit() override;
};


//Lighting shader to perform skybox reflections and blinn-phong lighting
class SkyboxLightingShader : public ShaderProgram2 {
private:
	SkyBox* sceneSkyBox;
	StaticFrameBuffer* skyboxLightingBuffer;
	void onNextPass(int shaderStage, unsigned int shaderID) override;
public:
	void onInit() override;
	void destroy();
	inline void setSkybox(SkyBox& skybox) { sceneSkyBox = &skybox; };
};