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

	//Send a boolean value represetned by an integer to the shader
	void setBool(const char* attr, bool value);
	//Send a float value to the shader
	void setFloat(const char* attr, float value);
	//Send an integer value to the shader
	void setInt(const char* attr, int value);
	//Send a 3D vector to the shader
	void setVec3(const char* attr, glm::vec3 value);
	//Send a 4D vector to the shader
	void setVec4(const char* attr, glm::vec4 value);
	//Send a 4x4 matrix to the shader
	void setMat4(const char* attr, glm::mat4 value);
};