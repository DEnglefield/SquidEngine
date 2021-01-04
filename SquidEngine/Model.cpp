

#include <iostream>
#include "Model.h"



//Create a model with an initial position and model file
Model::Model(float x, float y, float z, const char* modelFile) : Object3D(x,y,z) {
	centerOfMass = glm::vec3(0.0f);
	loadModel(modelFile);
}


//Free up buffers used by meshes within this model
void Model::destroy() {
	for (int i = 0; i < meshes.size(); ++i) {
		meshes[i].destroy();
	}
}


//Load a model from file populating included meshes
void Model::loadModel(const char* modelFile) {
	
	Assimp::Importer importer;
	//Import the model file as triangles and generate normals
	const aiScene* modelScene = importer.ReadFile(modelFile, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

	//Check if model was loaded successfully
	if (!modelScene || modelScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !modelScene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	std::string stringFile(modelFile);
	directory = stringFile.substr(0, stringFile.find_last_of('/'));

	//Read master assimp node for meshes
	readAssimpNode(modelScene->mRootNode, modelScene);

	for (int i = 0; i < meshes.size(); ++i) {
		setPosition(getPosition().x, getPosition().y, getPosition().z);
	}

	centerModel();
}


//Find meshes within the model file tree structure
void Model::readAssimpNode(aiNode* treeNode, const aiScene* modelScene) {

	//Read each mesh in this node and add it to the model
	for (unsigned int i = 0; i < treeNode->mNumMeshes; i++) {
		aiMesh* mesh = modelScene->mMeshes[treeNode->mMeshes[i]];
		createMesh(mesh, modelScene);
		
	}

	//Get child node
	for (unsigned int i = 0; i < treeNode->mNumChildren; i++) {
		readAssimpNode(treeNode->mChildren[i], modelScene);
	}
}


//Generate a shape from a assimp mesh and add it to this model
void Model::createMesh(aiMesh* mesh, const aiScene* modelScene) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;


	//Get each vertex in the mesh
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		//Get vertex position
		glm::vec3 pos(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

		//Get vertex normal
		glm::vec3 normal(0, 0, 0);
		if (mesh->HasNormals()) {
			normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		}

		//Get texture coordinates
		glm::vec2 texUV(0, 0);
		if (mesh->mTextureCoords[0]) {
			texUV = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}

		//Create vertex instance and add it to the mesh
		vertices.push_back(Vertex(pos, normal, texUV));
	}

	std::cout << "Read all vertices" << std::endl;

	//Add face descriptions to mesh
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}


	Shape newShape(vertices, indices);

	//Read mesh material properties and add to the shape
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = modelScene->mMaterials[mesh->mMaterialIndex];

		aiColor3D ambient(0.f, 0.f, 0.f);
		aiColor3D diffuse(0.f, 0.f, 0.f);
		aiColor3D specular(0.f, 0.f, 0.f);
		float shininess = 8;
		float alpha = 1;

		material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
		material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
		material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
		material->Get(AI_MATKEY_SHININESS, shininess);
		material->Get(AI_MATKEY_OPACITY, alpha);

		glm::vec3 matAmbient(ambient.r, ambient.g, ambient.b);
		glm::vec3 matDiffuse(diffuse.r, diffuse.g, diffuse.b);
		glm::vec3 matSpecular(specular.r, specular.g, specular.b);

		Material meshMaterial;
		meshMaterial.highlight = shininess;
		meshMaterial.opacity = alpha;

		//Get diffuse maps
		
		std::vector<Texture> diffuseTextures = getMeshMaterialTextures(material, aiTextureType_DIFFUSE);
		if (diffuseTextures.size() > 0) {
			meshMaterial.materialTextures.insert(meshMaterial.materialTextures.end(), diffuseTextures.begin(), diffuseTextures.end());
		}
		else {
			meshMaterial.addTexture(Texture(matAmbient.r,matAmbient.g,matAmbient.b,1,1,TEXTURE_DIFFUSE_MAP));
		}


		//Get specular maps
		std::vector<Texture> specularTextures = getMeshMaterialTextures(material, aiTextureType_SPECULAR);
		if (specularTextures.size() > 0) {
			meshMaterial.materialTextures.insert(meshMaterial.materialTextures.end(), specularTextures.begin(), specularTextures.end());
		}
		else {
			meshMaterial.addTexture(Texture(matSpecular.r, matSpecular.g, matSpecular.b, 1, 1, TEXTURE_SPECULAR_MAP));
		}
		

		newShape.setMaterial(meshMaterial);
	}
	else {
		std::cout << "Mesh with no material detected" << std::endl;
	}

	//If mesh had no normals generate triangle normals
	if (!mesh->HasNormals()) { newShape.computeNormals(); }

	meshes.push_back(newShape);
}


//Get the material textures and properties for the material assimp node
std::vector<Texture> Model::getMeshMaterialTextures(aiMaterial* mat, aiTextureType type) {
	std::vector<Texture> textures;

	//Read all mesh textures of the given texture type
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);

		//Check if texture has already been loaded in model and use that instead
		for (unsigned int j = 0; j < meshTextures.size(); j++)
		{
			if (std::strcmp(meshTextures[j].getImagePath().c_str(), str.C_Str()) == 0) {
				textures.push_back(meshTextures[j]);
				continue;
			}
		}

		//Load new texture and add to model
		std::string textureFile(directory);
		textureFile.append("\\");
		textureFile.append(str.C_Str());
		
		int textureType = 0;
		switch (type) {
		case aiTextureType_DIFFUSE: textureType = TEXTURE_DIFFUSE_MAP; break;
		case aiTextureType_SPECULAR: textureType = TEXTURE_SPECULAR_MAP; break;
		case aiTextureType_NORMALS: textureType = TEXTURE_NORMAL_MAP; break;
		default:textureType = TEXTURE_COLOUR_BUFFER; break;
		}

		Texture texture(textureFile.c_str(), textureType);
		textures.push_back(texture);
		meshTextures.push_back(texture);

	}
	return textures;
}


//Calculate the center of the model and move it to center of model coordinates
void Model::centerModel() {

	centerOfMass = glm::vec3(0, 0, 0);
	unsigned int numVertices = 0;

	//Calculate center of mass across all meshes
	for (int i = 0; i < meshes.size(); ++i) {
		numVertices += meshes[i].shapeVertices.size();
		for (int j = 0; j < meshes[i].shapeVertices.size(); ++j) {
			centerOfMass += meshes[i].shapeVertices[0].pos;
		}
	}

	centerOfMass /= numVertices;

	translate(-centerOfMass.x, -centerOfMass.y, -centerOfMass.z);
}



//Set the position of the model
void Model::setPosition(float x, float y, float z) {
	for (int i = 0; i < meshes.size(); ++i) {
		glm::vec3 newMeshPosition = glm::vec3(x, y, z) + centerOfMass + (meshes[i].getPosition() - centerOfMass);
		meshes[i].setPosition(newMeshPosition.x, newMeshPosition.y, newMeshPosition.z);
	}

	Object3D::setPosition(centerOfMass.x + x, centerOfMass.y + y, centerOfMass.z + z);
}


//Get the position of the model
glm::vec3 Model::getPosition() { return centerOfMass + Object3D::getPosition(); }

//Scale the model on the chosen axis
void Model::setScale(float x, float y, float z) {
	for (int i = 0; i < meshes.size(); ++i) {
		glm::vec3 newMeshPosition = (meshes[i].getPosition() - centerOfMass);
		meshes[i].setPosition(newMeshPosition.x * x, newMeshPosition.y * y, newMeshPosition.z * z);
		meshes[i].setScale(x,y,z);
	}

	Object3D::setScale(x, y, z);
}

//Get the current scale of the model
glm::vec3 Model::getScale() { return Object3D::getScale(); }

//Add a rotation to the model
void Model::rotate(float angle, float x, float y, float z) {
	for (int i = 0; i < meshes.size(); ++i) {
		glm::vec3 meshPos = meshes[i].getPosition();
		meshes[i].setPosition(0,0,0);
		meshes[i].rotate(angle,x,y,z);
		meshes[i].setPosition(meshPos.x, meshPos.y, meshPos.z);
	}

	Object3D::rotate(angle, x, y, z);
}


//Move the model along the given axis
void Model::translate(float x, float y, float z) {
	for (int i = 0; i < meshes.size(); ++i) {
		meshes[i].translate(x, y, z);
	}

	Object3D::translate(x,y,z);
}


//Set visability of the model and contained shapes
void Model::setVisable(bool state) { 
	for (int i = 0; i < meshes.size(); ++i) {
		meshes[i].setVisable(state);
	}
}





