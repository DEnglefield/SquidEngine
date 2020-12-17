#pragma once

#include "ShaderProgram2.h"

class BasicLightingShader: public ShaderProgram2 {
private:
	void onNextPass(int shaderStage, unsigned int shaderID) override;
public:
	void onInit() override;
	
};