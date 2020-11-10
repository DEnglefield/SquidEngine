
#include "Model.h"



//Create a model with an initial position and model file
Model::Model(float x, float y, float z, const char* modelFile) : Object3D(x,y,z) {
	centerOfMass = glm::vec3(0.0f);
	loadModel(modelFile);
}


//Free up buffers used by meshes within this model
void Model::destroyModel() {
	for (int i = 0; i < meshes.size(); ++i) {
		meshes[i].destroyShape();
	}
}


//Load a model from file populating included meshes
void Model::loadModel(const char* modelFile) {
	
	Assimp::Importer importer;
	//Import the model file as triangles and generate normals
	const aiScene* modelScene = importer.ReadFile(modelFile, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

	if (!modelScene || modelScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !modelScene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	std::string stringFile(modelFile);
	std::string  directory = stringFile.substr(0, stringFile.find_last_of('/'));

	readAssimpNode(modelScene->mRootNode, modelScene);

	centerModel();
}


//Find meshes within the model file tree structure
void Model::readAssimpNode(aiNode* treeNode, const aiScene* modelScene) {
	for (unsigned int i = 0; i < treeNode->mNumMeshes; i++) {
		aiMesh* mesh = modelScene->mMeshes[treeNode->mMeshes[i]];
		meshes.push_back(createMesh(mesh, modelScene));
	}

	for (unsigned int i = 0; i < treeNode->mNumChildren; i++) {
		readAssimpNode(treeNode->mChildren[i], modelScene);
	}
}


//Generate a shape from a assimp mesh
Shape Model::createMesh(aiMesh* mesh, const aiScene* modelScene) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		glm::vec3 pos(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

		glm::vec3 normal(0, 0, 0);
		if (mesh->HasNormals()) {
			normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		}

		glm::vec2 texUV(0, 0);
		if (mesh->mTextureCoords[0]) {
			texUV = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}

		vertices.push_back(Vertex(pos, normal, texUV));
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}


	return Shape(vertices, indices);
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



//Draw the model and all included shapes/meshes
void Model::draw(ShaderProgram& shader) {
	for (int i = 0; i < meshes.size(); ++i) {
		meshes[i].draw(shader);
	}
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
