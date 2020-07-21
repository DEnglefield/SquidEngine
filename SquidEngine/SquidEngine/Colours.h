#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct ColourList {
	glm::vec4 WHITE = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 BLACK = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	glm::vec4 RED = glm::vec4(0.0f,1.0f,0.0f, 1.0f);
	glm::vec4 GREEN = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 BLUE = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	glm::vec4 DarkGrey = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	glm::vec4 LightGrey = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
	glm::vec4 TURQUOISE = glm::vec4(0.0f, 0.5f, 0.5f, 1.0f);
	glm::vec4 YELLOW = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 ORANGE = glm::vec4(1.0f, 0.7f, 0.0f, 1.0f);
	glm::vec4 PURPLE = glm::vec4(0.5f, 0.0f, 0.5f, 1.0f);
	glm::vec4 CYAN = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 BROWN = glm::vec4(0.56f, 0.4f, 0.0f, 1.0f);
};