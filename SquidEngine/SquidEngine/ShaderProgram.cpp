
#include <fstream>
#include <string>
#include "SquidEngine.h"
#include "ShaderProgram.h"

using namespace std;

//Shader Program used to load GLSL code to run on the GPU
ShaderProgram::ShaderProgram(const char* vertexFile, const char* geometryFile, const char* fragmentFile) {
	//Make sure vertex and fragment shader files have been provided
	if (vertexFile == nullptr || fragmentFile == nullptr) {
		throw invalid_argument("Vertex and Fragment shader are not optional");
	}

	//Load vertex shader
	unsigned int vertexID = loadShader(vertexFile, GL_VERTEX_SHADER);
	if (vertexID != -1) {
		cout << "Vertex Shader Loaded Successfully" << endl;
	}

	//Load fragment shader
	unsigned int fragmentID = loadShader(fragmentFile, GL_FRAGMENT_SHADER);
	if (fragmentID != -1) {
		cout << "Fragment Shader Loaded Successfully" << endl;
	}

	//Load geometry shader is requested
	unsigned int geometryID = -1;
	if (geometryFile != nullptr) {
		geometryID = loadShader(geometryFile, GL_GEOMETRY_SHADER);
		if (geometryID != -1) {
			cout << "Geometry Shader Loaded Successfully" << endl;
		}
	}

	//Link the shaders to form the render pipeline
	ID = createShaderProgram(vertexID, geometryID, fragmentID);

	//Delete each shader as they have been linked
	//and are no longer needed
	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
	if (geometryID != -1) { glDeleteShader(geometryID); }
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
		if (shaderType == GL_VERTEX_SHADER) {
			cout << "Failed to compile vertex shader:" << endl;
		}

		if (shaderType == GL_GEOMETRY_SHADER) {
			cout << "Failed to compile geometry shader:" << endl;
		}

		if (shaderType == GL_FRAGMENT_SHADER) {
			cout << "Failed to compile fragment shader:" << endl;
		}
		
		cout << infoLog << endl;
		return -1;
	}
	
	return shaderID;
}

//Link the shader stages to form a shader program
//Geom should be -1 if not using geometry shader
unsigned int ShaderProgram::createShaderProgram(unsigned int vert, unsigned int geom, unsigned int frag) {
	//Create ID for the shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	//Attach Shaders
	glAttachShader(shaderProgram, vert);
	glAttachShader(shaderProgram, frag);
	if (geom != -1) { glAttachShader(shaderProgram, geom); }

	//Link the program to the application
	glLinkProgram(shaderProgram);

	int success;
	char infoLog[512];

	//Check if link was successful
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "Failed to link shaders:" << endl;
		cout << infoLog << endl;
		return -1;
	}

	cout << "Program Created" << endl;
	return shaderProgram;
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