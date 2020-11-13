
#include <fstream>
#include <string>
#include "SquidEngine.h"
#include "ShaderProgram.h"

using namespace std;

//Shader Program used to load GLSL code to run on the GPU
ShaderProgram::ShaderProgram(const char* vertexFile, const char* fragmentFile) {

	initLightHandles();

	//Load vertex shader
	vertexID = loadShader(vertexFile, GL_VERTEX_SHADER);
	if (vertexID != -1) {
		cout << "Vertex Shader Loaded Successfully" << endl;
	}

	//Load fragment shader
	fragmentID = loadShader(fragmentFile, GL_FRAGMENT_SHADER);
	if (fragmentID != -1) {
		cout << "Fragment Shader Loaded Successfully" << endl;
	}
}


bool ShaderProgram::attachGeometryShader(const char* geometryFile) {
	//Load geometry shader
	geometryID = loadShader(geometryFile, GL_GEOMETRY_SHADER);
	if (geometryID != -1) {
		cout << "Geometry Shader Loaded Successfully" << endl;
	}
	
	return (geometryID != -1);
}



//Set this program as the active program for rendering
void ShaderProgram::use() { glUseProgram(ID); }

//Open a shader file and read the text
string ShaderProgram::openShader(const char* fileName) {
	fstream file;

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


//Load a shader file and compile the GLSL code
unsigned int ShaderProgram::loadShader(const char* fileName, int shaderType) {
	//ID for this shader stage
	unsigned int shaderID;

	//Generate a unique shader ID
	shaderID = glCreateShader(shaderType);

	//Open the GLSL code
	string code = openShader(fileName);
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
		cout << infoLog << endl;
		if (shaderType == GL_VERTEX_SHADER) {
			throw exception("Failed to compile vertex shader");
		}

		if (shaderType == GL_GEOMETRY_SHADER) {
			throw exception("Failed to compile geometry shader");
		}

		if (shaderType == GL_FRAGMENT_SHADER) {
			throw exception("Failed to compile fragment shader");
		}
		
		return -1;
	}
	
	return shaderID;
}

//Link the shader stages to form a shader program
bool ShaderProgram::createShaderProgram() {
	//Create ID for the shader program
	ID = glCreateProgram();

	//Attach Shaders
	glAttachShader(ID, vertexID);
	glAttachShader(ID, fragmentID);
	if (geometryID != -1) { glAttachShader(ID, geometryID); }

	//Link the program to the application
	glLinkProgram(ID);

	int success;
	char infoLog[512];

	//Check if link was successful
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(ID, 512, NULL, infoLog);
		cout << "Failed to link shaders:" << endl;
		cout << infoLog << endl;
		return false;
	}

	cout << "Program Created" << endl;

	//Delete each shader as they have been linked and are no longer needed
	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
	if (geometryID != -1) {
		glDeleteShader(geometryID);
	}
	return true;
}

//Send a boolean value represetned by an integer to the shader
void ShaderProgram::setBool(const char* attr, bool value) {
	glUniform1i(glGetUniformLocation(ID, attr), value);
}

//Send a float value to the shader
void ShaderProgram::setFloat(const char* attr, float value) {
	glUniform1f(glGetUniformLocation(ID, attr), value);
}

//Send an integer value to the shader
void ShaderProgram::setInt(const char* attr, int value) {
	glUniform1i(glGetUniformLocation(ID, attr), value);
}


//Send a 3D vector to the shader
void ShaderProgram::setVec3(const char* attr, glm::vec3 value) {
	unsigned int vec3 = glGetUniformLocation(ID, attr);
	glUniform3fv(vec3, 1, glm::value_ptr(value));
}

//Send a 4D vector to the shader
void ShaderProgram::setVec4(const char* attr, glm::vec4 value) {
	unsigned int vec4 = glGetUniformLocation(ID, attr);
	glUniform4fv(vec4, 1, glm::value_ptr(value));
}

//Send a 4x4 matrix to the shader
void ShaderProgram::setMat4(const char* attr, glm::mat4 value) {
	unsigned int mat4 = glGetUniformLocation(ID, attr);
	glUniformMatrix4fv(mat4, 1, GL_FALSE, glm::value_ptr(value));
}


//Apply all properties of a material to the shader
void ShaderProgram::setMaterial(Material& material) {

	string propertyName = string(MATERIAL_UNIFORM);
	propertyName.append(".");

	glActiveTexture(GL_TEXTURE0 + DIFFUSE_MAP_TEXTURE_LAYER);
	glBindTexture(GL_TEXTURE_2D, material.diffuseTexture.getID());

	glActiveTexture(GL_TEXTURE0 + SPECULAR_MAP_TEXTURE_LAYER);
	glBindTexture(GL_TEXTURE_2D, material.specularTexture.getID());

	setFloat((propertyName + MATERIAL_REFLECTIVITY_UNIFORM).c_str(), material.highlight);
}


//Get the uniform name of a chosen material property
const char* ShaderProgram::getMaterialPropertyName(const char* property) {
	string propertyName = string(MATERIAL_UNIFORM);
	propertyName.append(".");
	propertyName.append(property);
	return propertyName.c_str();
}


//Send the active camera position to the shader
void ShaderProgram::setCameraPosition(glm::vec3 value) {
	setVec3(CAMERA_POSITION_UNIFORM, value);
}



//Initialize light source tracking values
void ShaderProgram::initLightHandles() {

	pointLightHandles.resize(MAX_POINT_LIGHTS);
	spotLightHandles.resize(MAX_SPOT_LIGHTS);
	directionalLightHandles.resize(MAX_DIRECTIONAL_LIGHTS);

	for (int i = 0; i < MAX_POINT_LIGHTS; ++i) {
		pointLightHandles[i] = i;
		//setBool(getIndexedUniform(ENABLED_POINT_LIGHTS_UNIFORM,i).c_str(),false);
	}

	for (int i = 0; i < MAX_SPOT_LIGHTS; ++i) {
		spotLightHandles[i] = i;
		//setBool(getIndexedUniform(ENABLED_SPOT_LIGHTS_UNIFORM, i).c_str(), false);
	}

	for (int i = 0; i < MAX_DIRECTIONAL_LIGHTS; ++i) {
		directionalLightHandles[i] = i;
		//setBool(getIndexedUniform(ENABLED_DIRECTIONAL_LIGHTS_UNIFORM, i).c_str(), false);
	}
}



//Add a point light to the scene taking the given slot
unsigned int ShaderProgram::addPointLight(PointLight& pointLight) {

	unsigned int newHandle = pointLightHandles.back();
	pointLightHandles.pop_back();

	string lightUniform = getIndexedUniform(POINT_LIGHT_UNIFORM,newHandle) + ".";

	setVec3((lightUniform + LIGHT_POSITION_UNIFORM).c_str(), pointLight.position);
	setVec3((lightUniform + LIGHT_AMBIENT_UNIFORM).c_str(), pointLight.ambient);
	setVec3((lightUniform + LIGHT_DIFFUSE_UNIFORM).c_str(), pointLight.diffuse);
	setVec3((lightUniform + LIGHT_SPECULAR_UNIFORM).c_str(), pointLight.specular);
	setFloat((lightUniform + LIGHT_LINEAR_FALL_OFF_UNIFORM).c_str(), pointLight.linearFallOff);
	setFloat((lightUniform + LIGHT_QUADRATIC_FALL_OFF_UNIFORM).c_str(), pointLight.quadraticFallOff);

	setBool(getIndexedUniform(ENABLED_POINT_LIGHTS_UNIFORM,newHandle).c_str(), true);

	return newHandle;
}


//Remove a point light from the scene
void ShaderProgram::removePointLight(unsigned int handle) {
	pointLightHandles.push_back(handle);
	setBool(getIndexedUniform(ENABLED_POINT_LIGHTS_UNIFORM, handle).c_str(), false);
}


//Add a spot light to the scene taking the given slot
unsigned int ShaderProgram::addSpotLight(SpotLight& spotLight) {

	unsigned int newHandle = pointLightHandles.back();
	pointLightHandles.pop_back();

	string lightUniform = getIndexedUniform(SPOT_LIGHT_UNIFORM, newHandle) + ".";

	setVec3((lightUniform + LIGHT_POSITION_UNIFORM).c_str(), spotLight.position);
	setVec3((lightUniform + LIGHT_DIRECTION_UNIFORM).c_str(), spotLight.direction);
	setVec3((lightUniform + LIGHT_AMBIENT_UNIFORM).c_str(), spotLight.ambient);
	setVec3((lightUniform + LIGHT_DIFFUSE_UNIFORM).c_str(), spotLight.diffuse);
	setVec3((lightUniform + LIGHT_SPECULAR_UNIFORM).c_str(), spotLight.specular);
	setFloat((lightUniform + LIGHT_LINEAR_FALL_OFF_UNIFORM).c_str(), spotLight.linearFallOff);
	setFloat((lightUniform + LIGHT_QUADRATIC_FALL_OFF_UNIFORM).c_str(), spotLight.quadraticFallOff);
	setFloat((lightUniform + LIGHT_INNER_CUT_OFF_UNIFORM).c_str(), spotLight.innerCutOff);
	setFloat((lightUniform + LIGHT_OUTER_CUT_OFF_UNIFORM).c_str(), spotLight.outerCutOff);

	setBool(getIndexedUniform(ENABLED_SPOT_LIGHTS_UNIFORM, newHandle).c_str(), true);

	return newHandle;
}


//Remove a spot light from the scene
void ShaderProgram::removeSpotLight(unsigned int handle) {
	spotLightHandles.push_back(handle);
	setBool(getIndexedUniform(ENABLED_SPOT_LIGHTS_UNIFORM, handle).c_str(), false);
}


//Add a directional light to the scene and return its handle
unsigned int ShaderProgram::addDirectionalLight(DirectionalLight& directionalLight) {

	unsigned int newHandle = pointLightHandles.back();
	pointLightHandles.pop_back();

	string lightUniform = getIndexedUniform(DIRECTIONAL_LIGHT_UNIFORM, newHandle) + ".";

	setVec3((lightUniform + LIGHT_DIRECTION_UNIFORM).c_str(), directionalLight.direction);
	setVec3((lightUniform + LIGHT_AMBIENT_UNIFORM).c_str(), directionalLight.ambient);
	setVec3((lightUniform + LIGHT_DIFFUSE_UNIFORM).c_str(), directionalLight.diffuse);
	setVec3((lightUniform + LIGHT_SPECULAR_UNIFORM).c_str(), directionalLight.specular);

	setBool(getIndexedUniform(ENABLED_DIRECTIONAL_LIGHTS_UNIFORM, newHandle).c_str(), true);

	return newHandle;
}



//Remove a directional light from the scene
void ShaderProgram::removeDirectionalLight(unsigned int handle) {
	directionalLightHandles.push_back(handle);
	setBool(getIndexedUniform(ENABLED_DIRECTIONAL_LIGHTS_UNIFORM, handle).c_str(), false);
}



//Get the uniform name of the given property of the given spot light for the chosen light of the chosen light source type
std::string ShaderProgram::getLightPropertyName(const char* lightType, const char* property, unsigned int handle) {
	std::string propertyName = getIndexedUniform(lightType, handle);
	propertyName.append(".");
	propertyName.append(property);
	return propertyName;
}


//Append a index string to a property uniform name
std::string ShaderProgram::getIndexedUniform(const char* property, int index) {
	std::string propertyName(property);
	propertyName.append("[");
	propertyName.append(std::to_string(index)+"]");
	return propertyName;
}


//Get the location ID for a uniform name
int ShaderProgram::getUniformLocation(const char* attr) {
	return glGetUniformLocation(ID, attr);
}