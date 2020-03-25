#pragma once

#include "Shape.h"

//Simple Square on the X and Y axis
class Square : public Shape {
public:
	Square(float x, float y, float z);
	void build();
};