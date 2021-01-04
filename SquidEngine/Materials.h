#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <list>
#include "Texture.h"



//Definition of material properties to be attached to an object to modify lighting
class Material {
protected:
	void initMaterial();
public:

	//std::list<Drawable*> boundShapes;

	std::vector<Texture> materialTextures;
	float highlight;
	float reflectivity, refractivity, refractiveIndex;
	float opacity;

	Material();
	Material(glm::vec3 matDiffuse, glm::vec3 matSpecular, float matHighlight);
	Material(Texture diffuseMap, Texture specularMap, float matHighlight);
	Material(const char* diffuseMap, const char* specularMap, float matHighlight);

	inline void addTexture(Texture texture) { materialTextures.push_back(texture); };

	void destroy();

};


/*
//List of pre-made materials
struct Materials {
	
	Materials();

	Material chrome;
	Material jade;
	Material emerald;
	Material ruby;
	Material pearl;
	Material gold;
	Material polishedGold;
	Material silver;
	Material polishedSilver;
	Material bronze;
	Material polishedBronze;
	Material copper;
	Material polishedCopper;
	Material tin;
	Material brass;
	Material obsidian;
	Material turquoise;
	Material redRubber;
	Material greenRubber;
	Material blueRubber;
	Material blackRubber;
	Material whiteRubber;
	Material cyanRubber;
	Material yellowRubber;
	Material redPlastic;
	Material greenPlastic;
	Material bluePlastic;
	Material blackPlastic;
	Material whitePlastic;
	Material cyanPlastic;
	Material yellowPlastic;
	Material water;
	

};

*/