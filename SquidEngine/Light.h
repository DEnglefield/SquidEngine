#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Base representation of a light source including effects on material illumination
struct Light {
	glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	glm::vec3 diffuse = glm::vec3(0.75f, 0.75f, 0.75f);
	glm::vec3 specular = glm::vec3(0.8f, 0.8f, 0.8f);
	bool isVisable = true;
	void setLightProperties(glm::vec3 lightAmbient, glm::vec3 lightDiffuse, glm::vec3 lightSpecular) {
		ambient = lightAmbient;
		diffuse = lightDiffuse;
		specular = lightSpecular;
	}
};



//A point light with falloff
//Set linear and quadratic falloff to 0 for infinite distance
struct PointLight : public Light {
	glm::vec3 position = glm::vec3(0.0f);
	float linearFallOff = 0.22f;
	float quadraticFallOff = 0.2f;
	inline PointLight(float posX, float posY, float posZ) 
		: position(glm::vec3(posX, posY, posZ)) {};
	inline PointLight(glm::vec3 lightPosition, float linFallOff, float quadFallOff)
		: position(lightPosition), linearFallOff(linFallOff), quadraticFallOff(quadFallOff) {};
};


//A directional light that ignores positions
struct DirectionalLight : public Light {
	glm::vec3 direction = glm::vec3(-0.5f);
	inline DirectionalLight(float dirX, float dirY, float dirZ) 
		: direction(glm::vec3(dirX, dirY, dirZ)) {};
	inline DirectionalLight(glm::vec3 lightDirection) : direction(lightDirection) {};

};


//Spot light to simulate focused light distribution
struct SpotLight : public Light {
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 direction = glm::vec3(-0.5f);
	float linearFallOff = 0.22f;
	float quadraticFallOff = 0.2f;
	float innerCutOff = glm::cos(glm::radians(10.5f));
	float outerCutOff = glm::cos(glm::radians(15.8f));

	inline SpotLight(float posX, float posY, float posZ, float dirX, float dirY, float dirZ)
		: position(glm::vec3(posX, posY, posZ)), direction(glm::vec3(dirX, dirY, dirZ)) {};

	inline SpotLight(glm::vec3 lightPosition, glm::vec3 lightDirection, float cutOffInner,
		float cutOffOuter, float linFallOff, float quadFallOff)
		: position(lightPosition), direction(lightDirection), linearFallOff(linFallOff), 
		quadraticFallOff(quadFallOff), innerCutOff(cutOffInner), outerCutOff(cutOffOuter) {};

	inline void lookAt(glm::vec3 target) { direction = position - target; };
};