#pragma once

#include "SquidEngine.h"
#include "Primitives.h"
#include "ShaderProgram.h"
#include <vector>

//Base class for any shape capable of being rendered in the scene
class Shape {
private:
	//List of vertices and indices that make the shape
	std::vector<Vertex> shapeVertices;
	std::vector<unsigned int> shapeIndices;
protected:
	//Create IDs for 
	//Vertex buffer object(VBO)
	//Element buffer object(EBO)
	//Vertex Attribute buffer(VAO)
	unsigned int VBO, EBO, VAO;

	//Model matrix to hold shape transformations
	glm::mat4 modelMatrix;

	//Compute the normal vectors using the list of vertices and indices
	void computeNormals(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices);

	//Add the vertices and indices to the vertex buffer
	void createBuffer(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices);

public:
	//Create a shape at the given position
	Shape(float x, float y, float z);
	~Shape();

	//Build the shape populating its vertices
	virtual void build() = 0;
	//Draw the shape on the screen
	void draw(ShaderProgram& shader);

	//Set the position of the shape
	void setPosition(float x, float y, float z);
	//Get the position of the shape
	glm::vec3 getPosition();
	
	//Scale the shape on the chosen axis
	void setScale(float x, float y, float z);
	//Get the current scale of the shape
	glm::vec3 getScale();
	//Add a rotation to the shape
	void rotate(float angle, float x, float y, float z);
	//Move the shape along the given axis
	void translate(float x, float y, float z);
};
