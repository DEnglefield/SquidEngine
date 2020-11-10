#pragma once

#include "SquidEngine.h"
#include "Light.h"
#include <vector>

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

#define LIGHT_AMBIENT_UNIFORM "ambient"
#define LIGHT_DIFFUSE_UNIFORM "diffuse"
#define LIGHT_SPECULAR_UNIFORM "specular"
#define LIGHT_LINEAR_FALL_OFF_UNIFORM "linearFallOff"
#define LIGHT_QUADRATIC_FALL_OFF_UNIFORM "quadraticFallOff"
#define LIGHT_INNER_CUT_OFF_UNIFORM "innerCutOff"
#define LIGHT_OUTER_CUT_OFF_UNIFORM "outerCutOff"
#define LIGHT_POSITION_UNIFORM "position"
#define LIGHT_DIRECTION_UNIFORM "direction"

#define POINT_LIGHT_UNIFORM "pointLights"
#define ENABLED_POINT_LIGHTS_UNIFORM "enabledPointLights"
#define SPOT_LIGHT_UNIFORM "spotLights"
#define ENABLED_SPOT_LIGHTS_UNIFORM "enabledSpotLights"
#define DIRECTIONAL_LIGHT_UNIFORM "directionalLights"
#define ENABLED_DIRECTIONAL_LIGHTS_UNIFORM "enabledDirectionalLights"

#define MAX_POINT_LIGHTS 8
#define MAX_SPOT_LIGHTS 8
#define MAX_DIRECTIONAL_LIGHTS 8

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

	//Point light handle lists
	std::vector<unsigned int> pointLightHandles;
	std::vector<unsigned int> spotLightHandles;
	std::vector<unsigned int> directionalLightHandles;

	//Initialize light source tracking values
	void initLightHandles();

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
	//Get the uniform name of a chosen material property
	const char* getMaterialPropertyName(const char* property);
	//Send the active camera position to the shader
	void setCameraPosition(glm::vec3 value);

	//Add a point light to the scene and return its handle
	unsigned int addPointLight(PointLight& pointLight);
	//Remove a point light from the scene
	void removePointLight(unsigned int handle);
	//Add a spot light to the scene and return its handle
	unsigned int addSpotLight(SpotLight& spotLight);
	//Remove a spot light from the scene
	void removeSpotLight(unsigned int handle);
	//Add a directional light to the scene and return its handle
	unsigned int addDirectionalLight(DirectionalLight& directionalLight);
	//Remove a directional light from the scene
	void removeDirectionalLight(unsigned int handle);
	//Get the uniform name of the given property for a chosen light of the chosen type
	std::string getLightPropertyName(const char* lightType, const char* property, unsigned int handle);

	//Get the location ID for a uniform name
	int getUniformLocation(const char* attr);

	//Append a index string to a property uniform name
	std::string getIndexedUniform(const char* property, int index);
};