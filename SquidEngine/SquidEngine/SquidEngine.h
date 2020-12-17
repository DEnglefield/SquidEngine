#pragma once
#include "Window.h"
#include "EngineInstance.h"


void setViewPorts(ViewPort* views, int numViews);
void createEngineWindow(int width, int height, const char* title);
void runEngine(EngineInstance& instance);
bool isWindowFocused();
