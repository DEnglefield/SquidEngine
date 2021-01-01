
#include "ShaderProgram2.h"
#include <iostream>
#include <fstream>
#include <string>
#include <glad\glad.h>


std::list<ShaderProgram2*> ShaderProgram2::sceneShaders;

//Load a shader file and compile the GLSL code
unsigned int ShaderProgram2::loadShader(const char* fileName, int shaderType) {
	//ID for this shader stage
	unsigned int shaderID;

	//Generate a unique shader ID
	shaderID = glCreateShader(shaderType);

	//Open the GLSL code
	std::string code = openShader(fileName);
	const char* shaderCode = code.c_str();

	//Attempt to compile the GLSL code
	glShaderSource(shaderID, 1, &shaderCode, NULL);
	glCompileShader(shaderID);

	int  success;
	char infoLog[512];
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

	//Check if compile failed
	if (!success) {

		glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
		std::cout << infoLog << std::endl;
		if (shaderType == GL_VERTEX_SHADER) {
			throw std::exception("Failed to compile vertex shader");
		}

		if (shaderType == GL_GEOMETRY_SHADER) {
			throw std::exception("Failed to compile geometry shader");
		}

		if (shaderType == GL_FRAGMENT_SHADER) {
			throw std::exception("Failed to compile fragment shader");
		}

		return -1;
	}

	return shaderID;
}


//Open a shader file and read the text
std::string ShaderProgram2::openShader(const char* fileName) {
	std::fstream file;

	std::string line;
	std::string data;
	file.open(fileName);

	//Read each line of the shader file
	while (std::getline(file, line)) {
		data += line + '\n';
	}

	file.close();

	return data;
}


//Link the shader stages to form a shader program
unsigned int ShaderProgram2::createShaderProgram() {
	//Create ID for the shader program
	unsigned int ID = glCreateProgram();

	//Attach Shaders
	glAttachShader(ID, vertexID);
	glAttachShader(ID, fragmentID);
	if (geometryID != -1) { glAttachShader(ID, geometryID); }

	//Link the program to the application
	glLinkProgram(ID);

	int success;
	char infoLog[1024];

	//Check if link was successful
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	
	if (!success) {
		//Check if link was successful
		glGetShaderInfoLog(ID, 1024, NULL, infoLog);
		std::cout << "Failed to link shaders: " << infoLog << std::endl;
		throw std::runtime_error("Couldn't create shader program");
	}

	std::cout << "Program Created" << std::endl;

	//Delete each shader as they have been linked and are no longer needed
	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
	if (geometryID != -1) {
		glDeleteShader(geometryID);
	}
	return ID;
}


void ShaderProgram2::draw(FrameBuffer& target) {
	targetBuffer = &target;
	target.use();
	for (int i = 0; i < shaderPasses.size(); ++i) {
		glUseProgram(shaderPasses[i]);
		onNextPass(i, shaderPasses[i]);
	}
}


void ShaderProgram2::destroy() {
	ShaderProgram2::sceneShaders.remove(this);
	for (int i = 0; i < shaderPasses.size(); ++i) {
		glDeleteProgram(shaderPasses[i]);
	}
}



//Create next shader pass
void ShaderProgram2::newShaderPass(const char* vertexFile, const char* fragmentFile) {
	if (writingMode) { throw std::runtime_error("Failed to create shader pass: Shader Pipeline is already in write mode"); }
	writingMode = true;

	//Load vertex shader
	vertexID = loadShader(vertexFile, GL_VERTEX_SHADER);
	if (vertexID != -1) {
		std::cout << "Vertex Shader Loaded Successfully" << std::endl;
	}

	//Load fragment shader
	fragmentID = loadShader(fragmentFile, GL_FRAGMENT_SHADER);
	if (fragmentID != -1) {
		std::cout << "Fragment Shader Loaded Successfully" << std::endl;
	}

	geometryID = -1;
}



//Attach an optional geometry shader
bool ShaderProgram2::attachGeometryShader(const char* geometryFile) {
	if (!writingMode) { throw std::runtime_error("Failed to add geometry shader: Shader Pipeline not in write mode"); }
	//Load geometry shader
	geometryID = loadShader(geometryFile, GL_GEOMETRY_SHADER);
	if (geometryID != -1) {
		std::cout << "Geometry Shader Loaded Successfully" << std::endl;
	}

	return (geometryID != -1);
}


//Finish creating a new shader pass ignoring future commands
int ShaderProgram2::finishShaderPass() {
	if (!writingMode) { throw std::runtime_error("Failed to finish shader pipeline: Shader Pipeline not in write mode"); }
	shaderPasses.push_back(createShaderProgram());
	writingMode = false;
	return shaderPasses.size()-1;
}


//Send a boolean value represetned by an integer to the shader
void ShaderProgram2::setBool(int shaderStage, const char* attr, bool value, int index) {
	glUseProgram(shaderPasses[shaderStage]);
	glUniform1i(glGetUniformLocation(shaderPasses[shaderStage], getIndexedUniform(attr, index).c_str()), value);
}

//Send a float value to the shader
void ShaderProgram2::setFloat(int shaderStage, const char* attr, float value, int index) {
	glUseProgram(shaderPasses[shaderStage]);
	glUniform1f(glGetUniformLocation(shaderPasses[shaderStage], getIndexedUniform(attr, index).c_str()), value);
}

//Send an integer value to the shader
void ShaderProgram2::setInt(int shaderStage, const char* attr, int value, int index) {
	glUseProgram(shaderPasses[shaderStage]);
	glUniform1i(glGetUniformLocation(shaderPasses[shaderStage], getIndexedUniform(attr, index).c_str()), value);
}

//Send a 3D vector to the shader
void ShaderProgram2::setVec3(int shaderStage, const char* attr, glm::vec3 value, int index) {
	glUseProgram(shaderPasses[shaderStage]);
	unsigned int loc = glGetUniformLocation(shaderPasses[shaderStage], getIndexedUniform(attr, index).c_str());
	glUniform3fv(loc, 1, glm::value_ptr(value));
}

//Send a 4D vector to the shader
void ShaderProgram2::setVec4(int shaderStage, const char* attr, glm::vec4 value, int index) {
	glUseProgram(shaderPasses[shaderStage]);
	unsigned int loc = glGetUniformLocation(shaderPasses[shaderStage], getIndexedUniform(attr, index).c_str());
	glUniform4fv(loc, 1, glm::value_ptr(value));
}

//Send a 4x4 matrix to the shader
void ShaderProgram2::setMat4(int shaderStage, const char* attr, glm::mat4 value, int index) {
	glUseProgram(shaderPasses[shaderStage]);
	unsigned int loc = glGetUniformLocation(shaderPasses[shaderStage], getIndexedUniform(attr, index).c_str());
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}


//Append a index string to a property uniform name
std::string ShaderProgram2::getIndexedUniform(const char* property, int index) {
	if (index == -1) { return property; }
	std::string propertyName(property);
	propertyName.append("[");
	propertyName.append(std::to_string(index) + "]");
	return propertyName;
}



void ShaderProgram2::addPointLight(PointLight& pointLight) {
	if (pointLights.size() >= MAX_POINT_LIGHTS - 1) {
		std::cout << "Didn't add point light (maxed lights reached)" << std::endl;
		return;
	}
	pointLights.push_back(&pointLight);
}



void ShaderProgram2::addDirectionalLight(DirectionalLight& dirLight) {
	if (directionalLights.size() >= MAX_DIRECTIONAL_LIGHTS - 1) {
		std::cout << "Didn't add directional light (maxed lights reached)" << std::endl;
		return;
	}
	directionalLights.push_back(&dirLight);
}


void ShaderProgram2::addSpotLight(SpotLight& spotLight) {
	if (spotLights.size() >= MAX_SPOT_LIGHTS - 1) {
		std::cout << "Didn't add spot light (maxed lights reached)" << std::endl;
		return;
	}
	spotLights.push_back(&spotLight);
}



//Get the uniform name of a chosen material property
const char* ShaderProgram2::getMaterialPropertyName(const char* property) {
	std::string propertyName = std::string(MATERIAL_UNIFORM);
	propertyName.append(".");
	propertyName.append(property);
	return propertyName.c_str();
}



void ShaderProgram2::useShapeMaterial(unsigned int shaderStage, Drawable& obj) {
	std::string propertyName = std::string(MATERIAL_UNIFORM);
	propertyName.append(".");

	int numDiffuseMaps = 0;
	int numSpecularMaps = 0;

	Material mat = obj.getMaterial();

	unsigned int shaderID = shaderPasses[shaderStage];

	for (int i = 0; i < mat.diffuseMaps.size(); ++i) {
		if (numDiffuseMaps >= MAX_DIFFUSE_MAPS) { continue; }

		int loc = glGetUniformLocation(shaderID,getIndexedUniform((
				propertyName + MATERIAL_DIFFUSE_MAPS_UNIFORM).c_str(), i).c_str());

		glActiveTexture(GL_TEXTURE0 + numDiffuseMaps);
		glBindTexture(GL_TEXTURE_2D, mat.diffuseMaps[i].getID());
		glUniform1i(loc, numDiffuseMaps);

		++numDiffuseMaps;
	}


	for (int i = 0; i < mat.specularMaps.size(); ++i) {
		if (numSpecularMaps >= MAX_SPECULAR_MAPS) { continue; }

		int loc = glGetUniformLocation(shaderID,getIndexedUniform((
				propertyName + MATERIAL_SPECULAR_MAPS_UNIFORM).c_str(), i).c_str());

		int textureLayer = numDiffuseMaps + numSpecularMaps;
		glActiveTexture(GL_TEXTURE0 + textureLayer);

		glBindTexture(GL_TEXTURE_2D, mat.specularMaps[i].getID());


		glUniform1i(loc, textureLayer);
		++numSpecularMaps;
	}


	setInt(shaderStage, (propertyName + MATERIAL_NUM_DIFFUSE_MAPS_UNIFORM).c_str(), numDiffuseMaps, -1);
	setInt(shaderStage, (propertyName + MATERIAL_NUM_SPECULAR_MAPS_UNIFORM).c_str(), numSpecularMaps, -1);
	setFloat(shaderStage, (propertyName + MATERIAL_HIGHLIGHT_UNIFORM).c_str(), mat.highlight, -1);
	setFloat(shaderStage, (propertyName + MATERIAL_REFLECTIVITY_UNIFORM).c_str(), mat.reflectivity, -1);
	setFloat(shaderStage, (propertyName + MATERIAL_REFRACTIVITY_UNIFORM).c_str(), mat.refractivity, -1);
	setFloat(shaderStage, (propertyName + MATERIAL_REFRACTIVE_INDEX_UNIFORM).c_str(), mat.refractiveIndex, -1);
	setFloat(shaderStage, (propertyName + MATERIAL_OPACITY_UNIFORM).c_str(), mat.opacity, -1);
}


void ShaderProgram2::addModel(Model& model){
	for (int i = 0; i < model.meshes.size(); ++i) {
		addShape(model.meshes[i]);
	}
}


void ShaderProgram2::removeModel(Model& model) {
	for (int i = 0; i < model.meshes.size(); ++i) {
		removeShape(model.meshes[i]);
	}
}



void ShaderProgram2::useLighting(unsigned int shaderStage) {
	
	int lightNum = 0;
	for (auto const& pointLight : pointLights) {
		if (!pointLight->isVisable) { continue; }
		std::string lightUniform = getIndexedUniform(POINT_LIGHT_UNIFORM, lightNum) + ".";
		setVec3(shaderStage,(lightUniform + LIGHT_POSITION_UNIFORM).c_str(), pointLight->position,-1);
		setVec3(shaderStage, (lightUniform + LIGHT_AMBIENT_UNIFORM).c_str(), pointLight->ambient,-1);
		setVec3(shaderStage, (lightUniform + LIGHT_DIFFUSE_UNIFORM).c_str(), pointLight->diffuse,-1);
		setVec3(shaderStage, (lightUniform + LIGHT_SPECULAR_UNIFORM).c_str(), pointLight->specular,-1);
		setFloat(shaderStage, (lightUniform + LIGHT_LINEAR_FALL_OFF_UNIFORM).c_str(), pointLight->linearFallOff,-1);
		setFloat(shaderStage, (lightUniform + LIGHT_QUADRATIC_FALL_OFF_UNIFORM).c_str(), pointLight->quadraticFallOff,-1);
		++lightNum;
	}
	setInt(shaderStage, NUM_POINT_LIGHTS_UNIFORM, lightNum,-1);

	lightNum = 0;
	for (auto const& directionalLight : directionalLights) {
		if (!directionalLight->isVisable) { continue; }
		std::string lightUniform = getIndexedUniform(DIRECTIONAL_LIGHT_UNIFORM, lightNum) + ".";
		setVec3(shaderStage, (lightUniform + LIGHT_DIRECTION_UNIFORM).c_str(), directionalLight->direction, -1);
		setVec3(shaderStage, (lightUniform + LIGHT_AMBIENT_UNIFORM).c_str(), directionalLight->ambient, -1);
		setVec3(shaderStage, (lightUniform + LIGHT_DIFFUSE_UNIFORM).c_str(), directionalLight->diffuse, -1);
		setVec3(shaderStage, (lightUniform + LIGHT_SPECULAR_UNIFORM).c_str(), directionalLight->specular, -1);
		++lightNum;
	}
	setInt(shaderStage, NUM_DIRECTIONAL_LIGHTS_UNIFORM, lightNum, -1);

	lightNum = 0;
	for (auto const& spotLight : spotLights) {
		if (!spotLight->isVisable) { continue; }
		std::string lightUniform = getIndexedUniform(SPOT_LIGHT_UNIFORM, lightNum) + ".";
		setVec3(shaderStage, (lightUniform + LIGHT_POSITION_UNIFORM).c_str(), spotLight->position, -1);
		setVec3(shaderStage, (lightUniform + LIGHT_DIRECTION_UNIFORM).c_str(), spotLight->direction, -1);
		setVec3(shaderStage, (lightUniform + LIGHT_AMBIENT_UNIFORM).c_str(), spotLight->ambient, -1);
		setVec3(shaderStage, (lightUniform + LIGHT_DIFFUSE_UNIFORM).c_str(), spotLight->diffuse, -1);
		setVec3(shaderStage, (lightUniform + LIGHT_SPECULAR_UNIFORM).c_str(), spotLight->specular, -1);
		setFloat(shaderStage, (lightUniform + LIGHT_LINEAR_FALL_OFF_UNIFORM).c_str(), spotLight->linearFallOff, -1);
		setFloat(shaderStage, (lightUniform + LIGHT_QUADRATIC_FALL_OFF_UNIFORM).c_str(), spotLight->quadraticFallOff, -1);
		setFloat(shaderStage, (lightUniform + LIGHT_INNER_CUT_OFF_UNIFORM).c_str(), spotLight->innerCutOff, -1);
		setFloat(shaderStage, (lightUniform + LIGHT_OUTER_CUT_OFF_UNIFORM).c_str(), spotLight->outerCutOff, -1);
		++lightNum;
	}
	setInt(shaderStage, NUM_SPOT_LIGHTS_UNIFORM, lightNum, -1);

}


void ShaderProgram2::useCamera(unsigned int shaderStage, Camera* cam) {
	setMat4(shaderStage, VIEW_MATRIX_UNIFORM, cam->getViewMatrix(), -1);
	setMat4(shaderStage, PROJECTION_MATRIX_UNIFORM, cam->getProjectionMatrix(), -1);
	setVec3(shaderStage, CAMERA_POSITION_UNIFORM, cam->getPosition(), -1);
}


void ShaderProgram2::drawShapes(unsigned int shaderStage) {

	for (auto const& obj : shaderShapes) {
		if (obj->isVisable()) {
			useShapeMaterial(shaderStage, *obj);
			setMat4(shaderStage, MODEL_MATRIX_UNIFORM, obj->getModelMatrix(), -1);
			obj->draw();
		}
	}
	
}


//Bind a uniform buffer block to given binding ID
unsigned int ShaderProgram2::bindUniformBlock(int shaderStage, unsigned int binding, const char* attr){
	unsigned int uniformLocation = glGetUniformBlockIndex(shaderPasses[shaderStage], attr);
	glUniformBlockBinding(shaderPasses[shaderStage], uniformLocation, binding);
	return uniformLocation;
}