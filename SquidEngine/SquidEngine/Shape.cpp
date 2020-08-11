
#include "Shape.h"

using namespace std;

//Create a shape at the given position
Shape::Shape(float x, float y, float z) {
	//Generate unique IDs for buffer objects
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	modelMatrix = glm::mat4(1.0f);

	glm::vec4 baseColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	setColour(baseColour);
	setPosition(x, y, z);
}

//Destructor for shape to delete buffer objects
Shape::~Shape() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}


//Draw the shape on the screen
void Shape::draw() {

	for (int i = 0; i < textureLayers.size(); ++i) {
		glActiveTexture(GL_TEXTURE0+i);
		glBindTexture(GL_TEXTURE_2D, textureLayers[i]);
	}
	
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, shapeIndices.size(), GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, DEFAULT_TEXTURE);
}


//Add a texture forming the next layer
void Shape::addTexture(unsigned int textureID) {
	textureLayers.push_back(textureID);
}


//Set the colour of the shape
void Shape::setColour(glm::vec4 &newColour) {
	shapeColour = newColour;
}


//Compute the normal vectors for all vertice in the shape
void Shape::computeNormals(vector<Vertex> &vertices, vector<unsigned int> &indices) {
	for (int i = 0; i < indices.size(); i+=3) {
		glm::vec3 BA = vertices[indices[i+1]].pos - vertices[indices[i]].pos;
		glm::vec3 CA = vertices[indices[i+2]].pos - vertices[indices[i]].pos;

		glm::vec3 triNormal = glm::cross(BA, CA);
		triNormal = glm::normalize(triNormal);
		vertices[indices[i]].normal = triNormal;
		vertices[indices[i+1]].normal = triNormal;
		vertices[indices[i+2]].normal = triNormal;
		
	}
}

//Add the vertices and indices to the vertex buffer
void Shape::createBuffer(vector<Vertex> &vertices, vector<unsigned int> &indices) {

	glBindVertexArray(VAO);

	//Populate vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);

	//Populate Element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);

	//Populate attribute buffer using vertex attributes
	Vertex::getAttributeDescription();

	glBindVertexArray(0);

	shapeVertices = vertices;
	shapeIndices = indices;

}


//Add a rotation to the shape
void Shape::rotate(float angle, float x, float y, float z) {
	modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), glm::vec3(x,y,z));
}


//Set the position of the shape
void Shape::setPosition(float x, float y, float z) {
	modelMatrix[3][0] = x;
	modelMatrix[3][1] = y;
	modelMatrix[3][2] = z;
	modelMatrix[3][3] = 1.0f;
}

//Get the position of the shape
glm::vec3 Shape::getPosition() {
	return glm::vec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]);
}

//Scale the shape on the chosen axis
void Shape::setScale(float x, float y, float z) {
	modelMatrix = glm::scale(modelMatrix, glm::vec3(x, y, z));
}

//Get the current scale of the shape
glm::vec3 Shape::getScale() {
	return glm::vec3(modelMatrix[0][0], modelMatrix[1][1], modelMatrix[2][2]);
}

//Move the shape along the given axis
void Shape::translate(float x, float y, float z) {
	modelMatrix = glm::translate(modelMatrix,glm::vec3(x,y,z));
}

//Return the shape's model matrix
glm::mat4 Shape::getModelMatrix() { return modelMatrix; }

//Get the shape's colour
glm::vec4 Shape::getColour() { return shapeColour; }