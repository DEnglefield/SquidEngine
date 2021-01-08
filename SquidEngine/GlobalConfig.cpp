
#include "GlobalConfig.h"

GlobalConfig renderConfig;

GlobalConfig::GlobalConfig() {
	defaultTextureID = 31;
	gamma = 2.2f;
	targetWidth = 1920;
	targetHeight = 1080;
	targetFPS = 15;
	enableMSAA = true;
	numSmaplesMSAA = 4;
}

