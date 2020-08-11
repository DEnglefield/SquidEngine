
#include "SquidEngine.h"
#include "Voxel.h"
#include "PerlinNoise.hpp"
#include "MarchingCubeLookup.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <cmath>

Voxel::Voxel(float x, float y, float z) : Shape(x, y, z) {}


void Voxel::build() {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	march(vertices, indices);

	createBuffer(vertices, indices);
}


void Voxel::populatePoints(int xDistributions, int yDistributions, int zDistributions) {
	xPoints = xDistributions;
	yPoints = yDistributions;
	zPoints = yDistributions;

	voxelPoints.clear();
	siv::PerlinNoise perlin(seed);
	double xFreq = xPoints / frequency;
	double yFreq = yPoints / frequency;
	double zFreq = zPoints / frequency;

	for (int x = 0; x < xPoints; ++x) {
		std::vector<std::vector<VoxelPoint>> heightVector;
		for (int y = 0; y < yPoints; ++y) {
			std::vector<VoxelPoint> depthVector;
			for (int z = 0; z < zPoints; ++z) {
				VoxelPoint newPoint;

				newPoint.pos.x = -0.5f + ((1 / (float)xPoints) * (float)x);
				newPoint.pos.y = -0.5f + ((1 / (float)yPoints) * (float)y);
				newPoint.pos.z = -0.5f + ((1 / (float)zPoints) * (float)z);

				//newPoint.value = (1-y) + perlin.accumulatedOctaveNoise3D_0_1(x / xFreq, y / yFreq, z / zFreq, octaves);
				newPoint.value = perlin.accumulatedOctaveNoise3D_0_1(x / xFreq, y / yFreq, z / zFreq, octaves);
				depthVector.push_back(newPoint);
			}
			heightVector.push_back(depthVector);
		}
		voxelPoints.push_back(heightVector);
	}

}


void Voxel::outputNoise(const char* fileName, int width, int height) {
	int writes = 0;
	siv::PerlinNoise perlin(seed);

	unsigned char* pixelValues = new unsigned char[(width * height) * 3];
	float xFreq = width / frequency;
	float yFreq = height / frequency;

	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {
			float perlinValue = perlin.accumulatedOctaveNoise2D_0_1(x / xFreq, y / yFreq, octaves);
			int pixelValue = perlinValue * 256;
			pixelValue = std::clamp(pixelValue, 0, 255);
			pixelValues[writes] = (unsigned char)pixelValue;
			pixelValues[writes + 1] = (unsigned char)pixelValue;
			pixelValues[writes + 2] = (unsigned char)pixelValue;
			writes += 3;
		}
	}

	stbi_write_png(fileName, width, height, 3, pixelValues, width * 3);
	delete[] pixelValues;
}


int Voxel::getTriangleConfigKey(std::vector<VoxelPoint>& cubePoints) {
	int lookupKey = 0;

	for (int i = 0; i < 8; ++i) {
		if (cubePoints[i].value >= surfaceLevel) {
			if (i == 0) { lookupKey++; }
			else { lookupKey += pow(2, i); }
		}
	}

	return lookupKey;
}


void Voxel::march(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) {

	for (int x = 0; x < xPoints - 1; ++x) {
		for (int y = 0; y < yPoints - 1; ++y) {
			for (int z = 0; z < zPoints - 1; ++z) {
				std::vector<VoxelPoint> cubePoints;
				cubePoints.push_back(voxelPoints[x][y][z]);
				cubePoints.push_back(voxelPoints[x + 1][y][z]);
				cubePoints.push_back(voxelPoints[x + 1][y][z + 1]);
				cubePoints.push_back(voxelPoints[x][y][z + 1]);
				cubePoints.push_back(voxelPoints[x][y + 1][z]);
				cubePoints.push_back(voxelPoints[x + 1][y + 1][z]);
				cubePoints.push_back(voxelPoints[x + 1][y + 1][z + 1]);
				cubePoints.push_back(voxelPoints[x][y + 1][z + 1]);

				int lookupKey = getTriangleConfigKey(cubePoints);

				std::vector<glm::vec3> trianglePoints;

				for (int i = 0; i < 16; ++i) {
					int edgeIndex = cubeConfigs[lookupKey][i];
					if (edgeIndex == -1) { break; }

					float point1Value = cubePoints[edgeConfigs[edgeIndex][0]].value;
					float point2Value = cubePoints[edgeConfigs[edgeIndex][1]].value;

					float min = point1Value;
					float max = point2Value;
					if (min > max) {
						max = point1Value;
						min = point2Value;
					}

					float roughnessCoefficient = 0.5f;

					float edgeDistance = 1-((0.5f * (min / max))* roughnessCoefficient);
					//std::cout << min << " is: " << (edgeDistance) << "% of: "  << max << std::endl;

					glm::vec3 edgePoint = (cubePoints[edgeConfigs[edgeIndex][0]].pos +
						cubePoints[edgeConfigs[edgeIndex][1]].pos) * edgeDistance;

					trianglePoints.push_back(edgePoint);
				}

				for (int i = 0; i < trianglePoints.size(); i += 3) {
					unsigned int numVertices = vertices.size();
					indices.push_back(numVertices);
					indices.push_back(numVertices + 1);
					indices.push_back(numVertices + 2);

					glm::vec3 side1 = trianglePoints[i + 1] - trianglePoints[i];
					glm::vec3 side2 = trianglePoints[i + 2] - trianglePoints[i];
					glm::vec3 normal = normalize(cross(side1, side2));

					if ((i + 1) % 6 == 0) {
						vertices.push_back(Vertex(trianglePoints[i], normal, glm::vec2(1, 1)));
						vertices.push_back(Vertex(trianglePoints[i + 1], normal, glm::vec2(1, 0)));
						vertices.push_back(Vertex(trianglePoints[i + 2], normal, glm::vec2(0, 0)));
					}
					else {
						vertices.push_back(Vertex(trianglePoints[i], normal, glm::vec2(0, 0)));
						vertices.push_back(Vertex(trianglePoints[i + 1], normal, glm::vec2(0, 1)));
						vertices.push_back(Vertex(trianglePoints[i + 2], normal, glm::vec2(1, 1)));
					}
				}

			}
		}
	}
}


void Voxel::setSeed(std::uint32_t newSeed) { seed = newSeed; }
std::uint32_t Voxel::getSeed() { return seed; }
void Voxel::setFrequency(float newFrequency) { frequency = newFrequency; }
float Voxel::getFrequency() { return frequency; }
void Voxel::setOctaves(int newOctaves) { octaves = newOctaves; }
int Voxel::getOctaves() { return octaves; }
void Voxel::setSurfaceLevel(float newSurfaceLevel) { surfaceLevel = newSurfaceLevel; }
float Voxel::getSurfaceLevel() { return surfaceLevel; }