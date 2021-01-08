#pragma once

//List of graphic quality and target options
struct GlobalConfig {
	GlobalConfig();
	int defaultTextureID;

	int targetWidth;
	int targetHeight;

	int shadowWidth;
	int shadowHeight;

	float targetFPS;

	bool enableMSAA;
	int numSmaplesMSAA;

	float gamma;
	bool enableVSync;
};

extern GlobalConfig renderConfig;
