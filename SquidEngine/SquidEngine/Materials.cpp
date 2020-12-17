
#include "Materials.h"

Material::Material() { }


Material::Material(glm::vec3 matDiffuse, glm::vec3 matSpecular, float matHighlight) {
	Texture diffuseTexture(matDiffuse.x, matDiffuse.y, matDiffuse.z, 1, 1);
	Texture specularTexture(matSpecular.x, matSpecular.y, matSpecular.z, 1, 1);
	addDiffuseMap(diffuseTexture);
	addSpecularMap(specularTexture);
	highlight = matHighlight;
	opacity = 1;
};


Material::Material(Texture diffuseMap, Texture specularMap, float matHighlight) {
	addDiffuseMap(diffuseMap);
	addSpecularMap(specularMap);
	highlight = matHighlight;
	opacity = 1;
};






void Material::destroy() {

	for (int i = 0; i < diffuseMaps.size(); ++i) {
		diffuseMaps[i].destroy();
	}

	for (int i = 0; i < specularMaps.size(); ++i) {
		specularMaps[i].destroy();
	}
	
}






/*
Materials::Materials() {
	chrome = Material(
		glm::vec3(0.4f, 0.4f, 0.4f),
		glm::vec3(0.774597f, 0.774597f, 0.774597f), 78);

	jade = Material(
		glm::vec3(0.54f, 0.89f, 0.63f),
		glm::vec3(0.316228f, 0.316228f, 0.316228f), 12.8f);

	emerald = Material(
		glm::vec3(0.07568f, 0.61424f, 0.07568f),
		glm::vec3(0.633f, 0.727811f, 0.633f), 78);

	ruby = Material(
		glm::vec3(0.61424f, 0.04136f, 0.04136f),
		glm::vec3(0.727811f, 0.626959f, 0.626959f), 76.8f);

	pearl = Material(
		glm::vec3(1.0f, 0.829f, 0.829f),
		glm::vec3(0.296648f, 0.296648f, 0.296648f), 11.264f);

	gold = Material(
		glm::vec3(0.75164f, 0.60648f, 0.22648f),
		glm::vec3(0.628281f, 0.555802f, 0.366065f), 51.2f);

	polishedGold = Material(
		glm::vec3(0.34615f, 0.3143f, 0.0903f),
		glm::vec3(0.797357f, 0.723991f, 0.208006f), 83.2f);

	silver = Material(
		glm::vec3(0.50754f, 0.50754f, 0.50754f),
		glm::vec3(0.508273f, 0.508273f, 0.508273f), 51.2f);

	polishedSilver = Material(
		glm::vec3(0.2775f, 0.2775f, 0.2775f),
		glm::vec3(0.773911f, 0.773911f, 0.773911f), 89.6f);

	bronze = Material(
		glm::vec3(0.714f, 0.4284f, 0.18144f),
		glm::vec3(0.393548f, 0.271906f, 0.166721f), 25.6f);

	polishedBronze = Material(
		glm::vec3(0.4f, 0.2368f, 0.1036f),
		glm::vec3(0.774597f, 0.458561f, 0.200621f), 76.8f);

	copper = Material(
		glm::vec3(0.7038f, 0.27048f, 0.0828f),
		glm::vec3(0.256777f, 0.137622f, 0.086014f), 21.8f);

	polishedCopper = Material(
		glm::vec3(0.5508f, 0.2118f, 0.066f),
		glm::vec3(0.580594f, 0.223257f, 0.0695701f), 51.2f);

	tin = Material(
		glm::vec3(0.427451f, 0.470588f, 0.541176f),
		glm::vec3(0.333333f, 0.333333f, 0.521569f), 9.84615f);

	brass = Material(
		glm::vec3(0.780392f, 0.568627f, 0.113725f),
		glm::vec3(0.992157f, 0.941176f, 0.807843f), 27.89743616f);

	obsidian = Material(
		glm::vec3(0.18275f, 0.17f, 0.22525f),
		glm::vec3(0.332741f, 0.328634f, 0.346435f), 38.4f);

	turquoise = Material(
		glm::vec3(0.396f, 0.74151f, 0.69102f),
		glm::vec3(0.297254f, 0.30829f, 0.306678f), 21.8f);

	redRubber = Material(
		glm::vec3(0.5f, 0.4f, 0.4f),
		glm::vec3(0.7f, 0.04f, 0.04f), 10);

	greenRubber = Material(
		glm::vec3(0.4f, 0.5f, 0.4f),
		glm::vec3(0.04f, 0.7f, 0.04f), 10);

	blueRubber = Material(
		glm::vec3(0.4f, 0.4f, 0.5f),
		glm::vec3(0.04f, 0.04f, 0.7f), 10);

	blackRubber = Material(
		glm::vec3(0.01f, 0.01f, 0.01f),
		glm::vec3(0.04f, 0.04f, 0.04f), 10);

	whiteRubber = Material(
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.7f, 0.7f, 0.7f), 10);

	cyanRubber = Material(
		glm::vec3(0.4f, 0.5f, 0.5f),
		glm::vec3(0.04f, 0.7f, 0.7f), 10);

	yellowRubber = Material(
		glm::vec3(0.5f, 0.5f, 0.4f),
		glm::vec3(0.7f, 0.7f, 0.04f), 10);

	redPlastic = Material(
		glm::vec3(0.5f, 0.0f, 0.0f),
		glm::vec3(0.7f, 0.6f, 0.6f), 32);

	greenPlastic = Material(
		glm::vec3(0.1f, 0.35f, 0.1f),
		glm::vec3(0.45f, 0.55f, 0.45f), 32);

	bluePlastic = Material(
		glm::vec3(0.1f, 0.1f, 0.35f),
		glm::vec3(0.45f, 0.45f, 0.55f), 32);

	blackPlastic = Material(
		glm::vec3(0.01f, 0.01f, 0.01f),
		glm::vec3(0.50f, 0.50f, 0.50f), 32);

	whitePlastic = Material(
		glm::vec3(0.55f, 0.55f, 0.55f),
		glm::vec3(0.70f, 0.70f, 0.70f), 32);

	cyanPlastic = Material(
		glm::vec3(0.0f, 0.50980392f, 0.50980392f),
		glm::vec3(0.50196078f, 0.50196078f, 0.50196078f), 32);

	yellowPlastic = Material(
		glm::vec3(0.5f, 0.5f, 0.0f),
		glm::vec3(0.60f, 0.60f, 0.50f), 32);

	water = Material(
		glm::vec3(0.7f, 0.568627f, 0.13f),
		glm::vec3(0.992157f, 0.86f, 0.7f), 30);
}
*/