#pragma once
#include "Primitives.h"
#include "Object3D.h"
#include "Materials.h"
#include <vector>


class Drawable : public Object3D {
private:
	//Flag to indicate object is visable
	bool visable;

	//Create IDs for 
	//Vertex buffer object(VBO)
	//Element buffer object(EBO)
	//Vertex Attribute buffer(VAO)
	unsigned int VBO, EBO, VAO;

	//Attached material for this shape
	Material material;

public:
	Drawable(float x, float y, float z);
	Drawable(float x, float y, float z, 
		std::vector<Vertex> vertices, std::vector<unsigned int> indices);

	//List of vertices and indices that make the shape
	std::vector<Vertex> shapeVertices;
	std::vector<unsigned int> shapeIndices;

	void draw();

	//Set visability of the shape
	inline void setVisable(bool state) { visable = state; };
	//Get shape visability
	inline bool isVisable() { return visable; };

	//Compute the normal vectors using the list of vertices and indices
	void computeNormals();

	//Add the vertices and indices to the vertex buffer
	void createBuffer(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

	//Set the material for this shape
	void inline setMaterial(Material shapeMaterial) { material = shapeMaterial; };

	//get this shape's material
	inline Material getMaterial() { return material; }


	void destroy();
};