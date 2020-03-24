#pragma once

#include "SquidEngine.h"

//Shader Program used to load GLSL code to run on the GPU
class ShaderProgram {
private:
	//Load a shader file and compile the GLSL code
	unsigned int loadShader(const char* fileName, int shaderType);
	//Open a shader file and read the text
	std::string openShader(const char* fileName);
protected:
	//Link the shader stages to form a shader program
	//Geom should be -1 if not using geometry shader
	unsigned int createShaderProgram(unsigned int vert, unsigned int geom, unsigned int frag);

public:
	//ID of the shader program
	unsigned int ID;
	//Create a shader program using the given shader files
	//Geometry shader should be nullptr if not used
	ShaderProgram(const char* vertexFile, const char* geometryFile, const char* fragmentFile);
	//Set this program as the active program for rendering
	void use();
};