#pragma once


struct GlobalConfig {
	GlobalConfig();
	int defaultTextureID;

	int targetWidth;
	int targetHeight;

	float targetFPS;

	bool enableMSAA;
	int numSmaplesMSAA;
};

extern GlobalConfig renderConfig;
