#pragma once

#include "SquidEngine.h"

#define MODEL_MATRIX_UNIFORM "modelMatrix"
#define WORLD_MATRIX_UNIFORM "worldMatrix"
#define VIEW_MATRIX_UNIFORM "viewMatrix"
#define PROJECTION_MATRIX_UNIFORM "projMatrix"
#define MATERIAL_UNIFORM "material"
#define MATERIAL_AMBIENT_UNIFORM "ambient"
#define MATERIAL_DIFFUSE_UNIFORM "diffuse"
#define MATERIAL_SPECULAR_UNIFORM "specular"
#define MATERIAL_REFLECTIVITY_UNIFORM "reflectivity"
#define CAMERA_POSITION_UNIFORM "cameraPos"

//Shader Program used to load GLSL code to run on the GPU
class ShaderProgram {
private:
	//Load a shader file and compile the GLSL code
	unsigned int loadShader(const char* fileName, int shaderType);
	//Open a shader file and read the text
	std::string openShader(const char* fileName);

	//Shader stage IDs
	unsigned int vertexID = -1;
	unsigned int fragmentID = -1;
	unsigned int geometryID = -1;

public:
	//ID of the shader program
	unsigned int ID = -1;

	//Create a shader program with a required vertex and fragment shader
	ShaderProgram(const char* vertexFile, const char* fragmentFile);

	//Attach an optional geometry shader
	bool attachGeometryShader(const char* geometryFile);
	//Link the shader stages to form a shader program
	bool createShaderProgram();

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

	//Apply all properties of a material to the shader
	void setMaterial(Material& material);
	//Change a specific material property
	void setMaterialProperty(const char* property, glm::vec3 value);
	//Send the active camera position to the shader
	void setCameraPosition(glm::vec3 value);

	//Get the location ID for a uniform name
	int getUniformLocation(const char* attr);
};