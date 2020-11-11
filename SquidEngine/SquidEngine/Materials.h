#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


//Definition of material properties to be attached to an object to modify lighting
struct Material {
	glm::vec3 ambient = glm::vec3(0.1f, 0.01f, 0.1f);
	glm::vec3 diffuse = glm::vec3(0.6f, 0.06f, 0.6f);
	glm::vec3 specular = glm::vec3(0.4f, 0.04f, 0.4f);
	float highlight = 32.0f;
	Material() {};
	Material(glm::vec3 matAmbient, glm::vec3 matDiffuse, glm::vec3 matSpecular, float matHighlight)
		: ambient(matAmbient), diffuse(matDiffuse), specular(matSpecular), highlight(matHighlight) {};
};


//List of per-made materials
struct Materials {
	static const Material chrome;
	static const Material jade;
	static const Material emerald;
	static const Material ruby;
	static const Material pearl;
	static const Material gold;
	static const Material polishedGold;
	static const Material silver;
	static const Material polishedSilver;
	static const Material bronze;
	static const Material polishedBronze;
	static const Material copper;
	static const Material polishedCopper;
	static const Material tin;
	static const Material brass;
	static const Material obsidian;
	static const Material turquoise;
	static const Material redRubber;
	static const Material greenRubber;
	static const Material blueRubber;
	static const Material blackRubber;
	static const Material whiteRubber;
	static const Material cyanRubber;
	static const Material yellowRubber;
	static const Material redPlastic;
	static const Material greenPlastic;
	static const Material bluePlastic;
	static const Material blackPlastic;
	static const Material whitePlastic;
	static const Material cyanPlastic;
	static const Material yellowPlastic;
	static const Material water;
};

