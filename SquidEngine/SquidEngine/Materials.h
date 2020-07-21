#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Material {
	glm::vec3 ambient = glm::vec3(0.1f, 0.01f, 0.1f);
	glm::vec3 diffuse = glm::vec3(0.6f, 0.06f, 0.6f);
	glm::vec3 specular = glm::vec3(0.4f, 0.04f, 0.4f);
	float highlight = 32.0f;
	Material(glm::vec3 matAmbient, glm::vec3 matDiffuse, glm::vec3 matSpecular, float matHighlight)
		: ambient(matAmbient), diffuse(matDiffuse), specular(matSpecular), highlight(matHighlight) {};
};

struct MaterialList {
	Material chrome = Material(
		glm::vec3(0.25f, 0.25f, 0.25f), 
		glm::vec3(0.4f, 0.4f, 0.4f), 
		glm::vec3(0.774597f, 0.774597f, 0.774597f), 78);

	Material jade = Material(
		glm::vec3(0.135f, 0.2225f, 0.1575f),
		glm::vec3(0.54f, 0.89f, 0.63f),
		glm::vec3(0.316228f, 0.316228f, 0.316228f), 12.8f);

	Material emerald = Material(
		glm::vec3(0.0215f, 0.1745f, 0.0215f),
		glm::vec3(0.07568f, 0.61424f, 0.07568f),
		glm::vec3(0.633f, 0.727811f, 0.633f), 78);

	Material ruby = Material(
		glm::vec3(0.1745f, 0.01175f, 0.01175f),
		glm::vec3(0.61424f, 0.04136f, 0.04136f),
		glm::vec3(0.727811f, 0.626959f, 0.626959f), 76.8f);

	Material pearl = Material(
		glm::vec3(0.25f, 0.20725f, 0.20725f),
		glm::vec3(1.0f, 0.829f, 0.829f),
		glm::vec3(0.296648f, 0.296648f, 0.296648f), 11.264f);

	Material gold = Material(
		glm::vec3(0.24725f, 0.1995f, 0.0745f),
		glm::vec3(0.75164f, 0.60648f, 0.22648f),
		glm::vec3(0.628281f, 0.555802f, 0.366065f), 51.2f);

	Material polishedGold = Material(
		glm::vec3(0.24725f, 0.2245f, 0.0645f),
		glm::vec3(0.34615f, 0.3143f, 0.0903f),
		glm::vec3(0.797357f, 0.723991f, 0.208006f), 83.2f);

	Material silver = Material(
		glm::vec3(0.19225f, 0.19225f, 0.19225f),
		glm::vec3(0.50754f, 0.50754f, 0.50754f),
		glm::vec3(0.508273f, 0.508273f, 0.508273f), 51.2f);

	Material polishedSilver = Material(
		glm::vec3(0.23125f, 0.23125f, 0.23125f),
		glm::vec3(0.2775f, 0.2775f, 0.2775f),
		glm::vec3(0.773911f, 0.773911f, 0.773911f), 89.6f);

	Material bronze = Material(
		glm::vec3(0.2125f, 0.1275f, 0.054f),
		glm::vec3(0.714f, 0.4284f, 0.18144f),
		glm::vec3(0.393548f, 0.271906f, 0.166721f), 25.6f);

	Material polishedBronze = Material(
		glm::vec3(0.25f, 0.148f, 0.06475f),
		glm::vec3(0.4f, 0.2368f, 0.1036f),
		glm::vec3(0.774597f, 0.458561f, 0.200621f), 76.8f);
	
	Material copper = Material(
		glm::vec3(0.19125f, 0.0735f, 0.0225f),
		glm::vec3(0.7038f, 0.27048f, 0.0828f),
		glm::vec3(0.256777f, 0.137622f, 0.086014f), 21.8f);

	Material polishedCopper = Material(
		glm::vec3(0.2295f, 0.08825f, 0.0275f),
		glm::vec3(0.5508f, 0.2118f, 0.066f),
		glm::vec3(0.580594f, 0.223257f, 0.0695701f), 51.2f);

	Material tin = Material(
		glm::vec3(0.105882f, 0.058824f, 0.113725f),
		glm::vec3(0.427451f, 0.470588f, 0.541176f),
		glm::vec3(0.333333f, 0.333333f, 0.521569f), 9.84615f);

	Material brass = Material(
		glm::vec3(0.329412f, 0.223529f, 0.027451f),
		glm::vec3(0.780392f, 0.568627f, 0.113725f),
		glm::vec3(0.992157f, 0.941176f, 0.807843f), 27.89743616f);

	Material obsidian = Material(
		glm::vec3(0.05375f, 0.05f, 0.06625f),
		glm::vec3(0.18275f, 0.17f, 0.22525f),
		glm::vec3(0.332741f, 0.328634f, 0.346435f), 38.4f);

	Material turquoise = Material(
		glm::vec3(0.1f, 0.18725f, 0.1745f),
		glm::vec3(0.396f, 0.74151f, 0.69102f),
		glm::vec3(0.297254f, 0.30829f, 0.306678f), 21.8f);

	Material redRubber = Material(
		glm::vec3(0.05f, 0.0f, 0.0f),
		glm::vec3(0.5f, 0.4f, 0.4f),
		glm::vec3(0.7f, 0.04f, 0.04f), 10);

	Material greenRubber = Material(
		glm::vec3(0.0f, 0.05f, 0.0f),
		glm::vec3(0.4f, 0.5f, 0.4f),
		glm::vec3(0.04f, 0.7f, 0.04f), 10);

	Material blueRubber = Material(
		glm::vec3(0.0f, 0.0f, 0.05f),
		glm::vec3(0.4f, 0.4f, 0.5f),
		glm::vec3(0.04f, 0.04f, 0.7f), 10);

	Material blackRubber = Material(
		glm::vec3(0.02f, 0.02f, 0.02f),
		glm::vec3(0.01f, 0.01f, 0.01f),
		glm::vec3(0.04f, 0.04f, 0.04f), 10);

	Material whiteRubber = Material(
		glm::vec3(0.05f, 0.05f, 0.05f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.7f, 0.7f, 0.7f), 10);

	Material cyanRubber = Material(
		glm::vec3(0.0f, 0.05f, 0.05f),
		glm::vec3(0.4f, 0.5f, 0.5f),
		glm::vec3(0.04f, 0.7f, 0.7f), 10);

	Material yellowRubber = Material(
		glm::vec3(0.05f, 0.05f, 0.0f),
		glm::vec3(0.5f, 0.5f, 0.4f),
		glm::vec3(0.7f, 0.7f, 0.04f), 10);

	Material redPlastic = Material(
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.5f, 0.0f, 0.0f),
		glm::vec3(0.7f, 0.6f, 0.6f), 32);

	Material greenPlastic = Material(
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.1f, 0.35f, 0.1f),
		glm::vec3(0.45f, 0.55f, 0.45f), 32);

	Material bluePlastic = Material(
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.1f, 0.1f, 0.35f),
		glm::vec3(0.45f, 0.45f, 0.55f), 32);

	Material blackPlastic = Material(
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.01f, 0.01f, 0.01f),
		glm::vec3(0.50f, 0.50f, 0.50f), 32);

	Material whitePlastic = Material(
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.55f, 0.55f, 0.55f),
		glm::vec3(0.70f, 0.70f, 0.70f), 32);

	Material cyanPlastic = Material(
		glm::vec3(0.0f, 0.1f, 0.06f),
		glm::vec3(0.0f, 0.50980392f, 0.50980392f),
		glm::vec3(0.50196078f, 0.50196078f, 0.50196078f), 32);

	Material yellowPlastic = Material(
		glm::vec3(0.0f, 0.1f, 0.06f),
		glm::vec3(0.5f, 0.5f, 0.0f),
		glm::vec3(0.60f, 0.60f, 0.50f), 32);

	Material water = Material(
		glm::vec3(0.0f, 0.45f, 0.98f),
		glm::vec3(0.7f, 0.568627f, 0.13f),
		glm::vec3(0.992157f, 0.86f, 0.7f), 30);
};