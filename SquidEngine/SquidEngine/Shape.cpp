

#include <glad\glad.h>
#include "Shape.h"

//Create a shape with mesh data
Shape::Shape(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) 
	: Drawable(0,0,0, vertices, indices) {}

//Create a shape with mesh data at the given position
Shape::Shape(float x, float y, float z, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) 
	: Drawable(x,y,z,vertices,indices) {}



//Destroy this shape de-coupling it from it's material
void Shape::destroy() {
}