
#include <fstream>
#include <string>
#include "SquidEngine.h"
#include "ShaderProgram.h"

using namespace std;

//Shader Program used to load GLSL code to run on the GPU
ShaderProgram::ShaderProgram(const char* vertexFile, const char* fragmentFile) {

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
		cout << infoLog << endl;
		glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
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
	glUniform1i(glGetUniformLocation(ID, attr), (int)value);
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
	setMaterialProperty(MATERIAL_AMBIENT_UNIFORM, material.ambient);
	setMaterialProperty(MATERIAL_DIFFUSE_UNIFORM, material.diffuse);
	setMaterialProperty(MATERIAL_SPECULAR_UNIFORM, material.specular);
	setMaterialProperty(MATERIAL_REFLECTIVITY_UNIFORM, glm::vec3(material.highlight));
}


//Change a specific material property
void ShaderProgram::setMaterialProperty(const char* property, glm::vec3 value) {
	string propertyName = string(MATERIAL_UNIFORM);
	propertyName.append(".");
	propertyName.append(property);
	
	if (property != MATERIAL_REFLECTIVITY_UNIFORM) {
		setVec3(propertyName.c_str(), value);
	} else {
		setFloat(propertyName.c_str(), value.x);
	}
}


//Send the active camera position to the shader
void ShaderProgram::setCameraPosition(glm::vec3 value) {
	setVec3(CAMERA_POSITION_UNIFORM, value);
}


//Get the location ID for a uniform name
int ShaderProgram::getUniformLocation(const char* attr) {
	return glGetUniformLocation(ID, attr);
}