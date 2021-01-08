#pragma once

#include "Window.h"
#include "EngineInstance.h"

//Update the list of screen partitionst to apply to the output framebuffer
void setViewPorts(ViewPort* views, int numViews);
//Initialize the application window with default options
void createEngineWindow(int width, int height, const char* title);
//Open the window and initialize OpenGL context beginning render loop
void runEngine(EngineInstance& instance);
//Get if the window is currently active on the host OS
bool isWindowFocused();
