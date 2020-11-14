
#include "Shape.h"

using namespace std;

//Create a shape with mesh data
Shape::Shape(std::vector<Vertex> vertices, std::vector<unsigned int> indices) : Object3D(0,0,0) {
	//setMaterial(Materials::pearl);
	createBuffer(vertices, indices);
}

//Create a shape with mesh data at the given position
Shape::Shape(float x, float y, float z, std::vector<Vertex> vertices, std::vector<unsigned int> indices) 
	: Object3D(x,y,z) {
	//setMaterial(Materials::pearl);
	createBuffer(vertices, indices);
}

//Destroy buffers for this shape
void Shape::destroyShape() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}


//Draw the shape on the screen
void Shape::draw(ShaderProgram& shader) {

	/*
	for (int i = 0; i < textureLayers.size(); ++i) {
		glActiveTexture(GL_TEXTURE0+i);
		glBindTexture(GL_TEXTURE_2D, textureLayers[i]);
	}
	*/

	shader.setMat4(MODEL_MATRIX_UNIFORM, modelMatrix);
	shader.setMaterial(material);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, shapeIndices.size(), GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, defaultTextureID);

}


//Set the material for this shape
void Shape::setMaterial(Material shapeMaterial) { material = shapeMaterial; }

//get this shape's material
Material Shape::getMaterial() { return material; }


//Compute the normal vectors for all vertice in the shape
//Slow and not smooth, use one-ring in geom or compute shader
void Shape::computeNormals() {
	for (int i = 0; i < shapeIndices.size(); i+=3) {
		glm::vec3 BA = shapeVertices[shapeIndices[i+1]].pos - shapeVertices[shapeIndices[i]].pos;
		glm::vec3 CA = shapeVertices[shapeIndices[i+2]].pos - shapeVertices[shapeIndices[i]].pos;

		glm::vec3 triNormal = glm::cross(BA, CA);
		triNormal = glm::normalize(triNormal);
		shapeVertices[shapeIndices[i]].normal = triNormal;
		shapeVertices[shapeIndices[i+1]].normal = triNormal;
		shapeVertices[shapeIndices[i+2]].normal = triNormal;
		
	}
}


//Add the vertices and indices to the vertex buffer
void Shape::createBuffer(vector<Vertex> &vertices, vector<unsigned int> &indices) {
	//Generate unique IDs for buffer objects
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

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


//Return the shape's model matrix
glm::mat4 Shape::getModelMatrix() { return modelMatrix; }
