
#include "GlobalConfig.h"

GlobalConfig renderConfig;

GlobalConfig::GlobalConfig() {
	renderConfig.defaultTextureID = 31;
	targetWidth = 1920;
	targetHeight = 1080;
	targetFPS = 60;
	enableMSAA = true;
	numSmaplesMSAA = 4;
}

