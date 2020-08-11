#pragma once
#include "SquidEngine.h"
#include "Shape.h"


struct VoxelPoint { 
	float value;
	glm::vec3 pos;
};


class Voxel : public Shape {
public:
	Voxel(float x, float y, float z);
	void build();
	void setSeed(std::uint32_t newSeed);
	std::uint32_t getSeed();
	void outputNoise(const char* fileName, int width, int height);
	void setFrequency(float newFrequency);
	float getFrequency();
	void setOctaves(int newOctaves);
	int getOctaves();
	void populatePoints(int xDistributions, int yDistributions, int zDistributions);
	void setSurfaceLevel(float newSurfaceLevel);
	float getSurfaceLevel(); 
protected:
	std::vector<std::vector<std::vector<VoxelPoint>>> voxelPoints;
	int xPoints, yPoints, zPoints;
	std::uint32_t seed = 12345;
	float frequency = 8.0f;
	int octaves = 8;
	float surfaceLevel = 0.5f;
private:
	int getTriangleConfigKey(std::vector<VoxelPoint>& cubePoints);
	void march(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
};