#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Shape.h"


class Cube : public Shape {
public:
	Cube(float x, float y, float z);
};