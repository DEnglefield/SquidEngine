#pragma once

#include "SquidEngine.h"
#include "Primitives.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Object3D.h"
#include <vector>

//Base class for any shape capable of being rendered in the scene
class Shape : public Object3D{
private:
	std::vector<unsigned int> textureLayers;
protected:
	//Create IDs for 
	//Vertex buffer object(VBO)
	//Element buffer object(EBO)
	//Vertex Attribute buffer(VAO)
	unsigned int VBO, EBO, VAO;

	//Attached material for this shape
	Material material;

	//Compute the normal vectors using the list of vertices and indices
	void computeNormals(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices);

public:

	//List of vertices and indices that make the shape
	std::vector<Vertex> shapeVertices;
	std::vector<unsigned int> shapeIndices;

	Shape(float x, float y, float z) : Object3D(x,y,z){};
	//Create a shape with mesh data
	Shape(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	//Create a shape with mesh data at the given position
	Shape(float x, float y, float z, std::vector<Vertex> vertices, std::vector<unsigned int> indices);

	//Destroy buffers for this shape
	void destroyShape();

	//Draw the shape on the screen
	void draw(ShaderProgram& shader);

	//Set the material for this shape
	void setMaterial(Material shapeMaterial);
	//get this shape's material
	Material getMaterial();

	//Add a texture forming the next layer
	void addTexture(unsigned int textureID);

	//Return the shape's model matrix
	glm::mat4 getModelMatrix();

	//Add the vertices and indices to the vertex buffer
	void createBuffer(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

};
