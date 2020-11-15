#pragma once

#include "SquidEngine.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Shape.h"
#include "Object3D.h"
#include "Materials.h"
#include <vector>
#include <filesystem>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//Representation of an imported model file including any contained meshes
class Model : public Object3D {
private:

	//List of shape objects representing model meshes
	std::vector<Shape> meshes;
	
	//List of textures included thoughout all meshes
	std::vector<Texture> meshTextures;

	//Center of mass for the entire model
	glm::vec3 centerOfMass;

	//Directory of the model
	std::string directory;

	//Calculate the center of the model and move it to center of model coordinates
	void centerModel();

	//Load a model from file populating included meshes
	void loadModel(const char* modelFile);

	//Find meshes within the model file tree structure
	void readAssimpNode(aiNode* treeNode, const aiScene* modelScene);

	//Get the material textures and properties for the material assimp node
	std::vector<Texture> getMeshMaterialTextures(aiMaterial* mat, aiTextureType type);

	//Generate a shape from a assimp mesh
	Shape createMesh(aiMesh* mesh, const aiScene* modelScene);


public:

	//Create a model with an initial position and model file
	Model(float x, float y, float z, const char* modelFile);

	//Free up buffers used by meshes within this model
	void destroyModel();

	//Draw the model and all included shapes/meshes
	void draw(ShaderProgram& shader);

	//Set the position of the model
	void setPosition(float x, float y, float z);
	//Get the position of the model
	glm::vec3 getPosition();

	//Scale the model on the chosen axis
	void setScale(float x, float y, float z);
	//Get the current scale of the model
	glm::vec3 getScale();
	//Add a rotation to the model
	void rotate(float angle, float x, float y, float z);
	//Move the model along the given axis
	void translate(float x, float y, float z);
};