#pragma once
#include <list>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "FrameBuffer.h"
#include "Drawable.h"
#include "Model.h"
#include "Light.h"
#include "Camera.h"


#define MODEL_MATRIX_UNIFORM "modelMatrix"
#define WORLD_MATRIX_UNIFORM "worldMatrix"
#define VIEW_MATRIX_UNIFORM "viewMatrix"
#define PROJECTION_MATRIX_UNIFORM "projMatrix"

#define MAX_DIFFUSE_MAPS 8 
#define MAX_SPECULAR_MAPS 8

#define MATERIAL_UNIFORM "material"
#define MATERIAL_NUM_DIFFUSE_MAPS_UNIFORM "numDiffuseMaps"
#define MATERIAL_DIFFUSE_MAPS_UNIFORM "diffuseMaps"
#define MATERIAL_NUM_SPECULAR_MAPS_UNIFORM "numSpecularMaps"
#define MATERIAL_SPECULAR_MAPS_UNIFORM "specularMaps"
#define MATERIAL_HIGHLIGHT_UNIFORM "highlight"
#define MATERIAL_REFLECTIVITY_UNIFORM "reflectivity"
#define MATERIAL_REFRACTIVITY_UNIFORM "refractivity"
#define MATERIAL_REFRACTIVE_INDEX_UNIFORM "refractiveIndex"
#define MATERIAL_OPACITY_UNIFORM "opacity"

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
#define NUM_POINT_LIGHTS_UNIFORM "numPointLights"
#define SPOT_LIGHT_UNIFORM "spotLights"
#define NUM_SPOT_LIGHTS_UNIFORM "numSpotLights"
#define DIRECTIONAL_LIGHT_UNIFORM "directionalLights"
#define NUM_DIRECTIONAL_LIGHTS_UNIFORM "numDirectionalLights"

#define CAMERA_POSITION_UNIFORM "cameraPos"

#define MAX_POINT_LIGHTS 8
#define MAX_SPOT_LIGHTS 8
#define MAX_DIRECTIONAL_LIGHTS 8



class ShaderProgram2 {

protected:
	std::vector<unsigned int> shaderPasses;

	std::list<Drawable*> shaderShapes;

	std::list<PointLight*> pointLights;
	std::list<DirectionalLight*> directionalLights;
	std::list<SpotLight*> spotLights;

	//Framebuffer to write final result to
	FrameBuffer* targetBuffer;

	//Temp data for building new shader pass
	unsigned int vertexID = -1;
	unsigned int fragmentID = -1;
	unsigned int geometryID = -1;
	FrameBuffer* passFrameBuffer;
	bool writingMode = false;

	Camera* mainCamera;

	//---Stuff to load and compile shader

	//Load a shader file and compile the GLSL code
	unsigned int loadShader(const char* fileName, int shaderType);
	//Open a shader file and read the text
	std::string openShader(const char* fileName);
	//Link the shader stages to form a shader program
	unsigned int createShaderProgram();

	void drawShapes(unsigned int shaderStage);
	void useLighting(unsigned int shaderStage);
	void useCamera(unsigned int shaderStage, Camera* cam);
	void useShapeMaterial(unsigned int shaderStage, Drawable& obj);
	virtual void onNextPass(int shaderStage, unsigned int shaderID) = 0;
	const char* getMaterialPropertyName(const char* property);

	//Create next shader pass
	void newShaderPass(const char* vertexFile, const char* fragmentFile);
	//Finish creating a new shader pass ignoring future commands
	int finishShaderPass();
	//Attach an optional geometry shader
	bool attachGeometryShader(const char* geometryFile);

public:
	static std::list<ShaderProgram2*> sceneShaders;
	//std::list<Material*> boundMaterials;

	inline ShaderProgram2() { ShaderProgram2::sceneShaders.push_back(this); }

	void draw(FrameBuffer& target);
	void destroy();

	virtual void onInit() = 0;


	inline void addShape(Drawable& obj) { shaderShapes.push_back(&obj); };
	inline void removeShape(Drawable& obj) { shaderShapes.remove(&obj); };
	void addModel(Model& obj);
	void removeModel(Model& obj);
	void addPointLight(PointLight& pointLight);
	void addDirectionalLight(DirectionalLight& dirLight);
	void addSpotLight(SpotLight& spotLight);

	inline void setMainCamera(Camera& cam) { mainCamera = &cam; }

	inline void removePointLight(PointLight& pointLight) { pointLights.remove(&pointLight); };
	inline void removeDirectionalLight(DirectionalLight& dirLight) { directionalLights.remove(&dirLight); };
	inline void removeSpotLight(SpotLight& spotLight) { spotLights.remove(&spotLight); };


	//Send a boolean value represetned by an integer to the shader
	void setBool(int shaderStage, const char* attr, bool value, int index);
	//Send a float value to the shader
	void setFloat(int shaderStage, const char* attr, float value, int index);
	//Send an integer value to the shader
	void setInt(int shaderStage, const char* attr, int value, int index);
	//Send a 3D vector to the shader
	void setVec3(int shaderStage, const char* attr, glm::vec3 value, int index);
	//Send a 4D vector to the shader
	void setVec4(int shaderStage, const char* attr, glm::vec4 value, int index);
	//Send a 4x4 matrix to the shader
	void setMat4(int shaderStage, const char* attr, glm::mat4 value, int index);

	//Append a index string to a property uniform name
	std::string getIndexedUniform(const char* property, int index);
	//Get the location ID for a uniform name
	inline int getUniformLocation(int shaderStage, const char* attr) { return glGetUniformLocation(shaderPasses[shaderStage], attr); }

	//Bind a uniform buffer block to given binding ID
	unsigned int bindUniformBlock(int shaderStage, unsigned int binding, const char* attr);
	
};