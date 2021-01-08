#pragma once

#include "ShaderProgram2.h"
#include "ScreenQuad.h"

//Shader pipeline template used for implementing post processing effects
class PostProcessShader : public ShaderProgram2 {
private:
	unsigned int inputRenderTextureID;
protected:
	ScreenQuad* screenQuad;
	void initPostProcess();
	inline void  drawQuad() { screenQuad->draw(); }
	inline unsigned int getRenderTexture() { return inputRenderTextureID; };
public:
	void setInputRender(unsigned int renderTexture);
	void destroy();
};


//A post process shader used to implement most common post processing effects
class CommonPostShader : public PostProcessShader {
private:
	void onNextPass(int shaderStage, unsigned int shaderID) override;
public:
	inline CommonPostShader() : PostProcessShader() { enableShader(false); }
	void onInit() override;
};