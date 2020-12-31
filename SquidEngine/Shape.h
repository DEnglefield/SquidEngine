#pragma once

#include <vector>
#include "Object3D.h"
#include "Drawable.h"

//Base class for any shape capable of being rendered in the scene
class Shape : public Drawable {

public:


	Shape(float x, float y, float z) : Drawable(x,y,z){};
	//Create a shape with mesh data
	Shape(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
	//Create a shape with mesh data at the given position
	Shape(float x, float y, float z, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);


	//Destroy this shape de-coupling it from it's material
	void destroy();
};
