#pragma once
#include <vector>
#include <string>
#include "Cube.h"

//A sampleable 3D cube texture build using a collection of 2D images
class CubeMap {
protected:
	unsigned int textureID;
	void generateCubeMap(std::vector<std::string>& cubeImages);

public:
	CubeMap(std::vector<std::string>& cubeImages);
	inline unsigned int getTextureID() { return textureID; };
	void destroy();
};


//A skybox which can be sampled as a cubemap
class SkyBox : public CubeMap, public Cube {
public:
	inline SkyBox(std::vector<std::string>& cubeImages) : 
		CubeMap(cubeImages), Cube(0, 0, 0) { setScale(100, 100, 100); };
	void destroy();
};