
#include "GlobalConfig.h"

GlobalConfig renderConfig;

GlobalConfig::GlobalConfig() {
	defaultTextureID = 31;
	gamma = 2.2f;
	targetWidth = 1920;
	targetHeight = 1080;
	shadowWidth = 1920;
	shadowHeight = 1080;
	targetFPS = 60;
	enableMSAA = true;
	numSmaplesMSAA = 4;
	enableVSync = false;
}

