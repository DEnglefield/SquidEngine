
#include "CommonPostShader.h"


void PostProcessShader::initPostProcess() {
	screenQuad = new ScreenQuad(0.1f, 0.1f, 0.9f, 0.9f);
}


void PostProcessShader::setInputRender(unsigned int renderTexture) {
	inputRenderTextureID = renderTexture;
	screenQuad->setTexture(renderTexture);
}


void PostProcessShader::destroy() {
	screenQuad->destroy();
	delete screenQuad;
	ShaderProgram2::destroy();
}


void CommonPostShader::onInit() {

	initPostProcess();

	newShaderPass(
		"Shaders/CommonPostShader/CommonPostShader.vert",
		"Shaders/CommonPostShader/CommonPostShader.frag");

	finishShaderPass();
}


void CommonPostShader::onNextPass(int shaderStage, unsigned int shaderID) {
	drawQuad();
}
