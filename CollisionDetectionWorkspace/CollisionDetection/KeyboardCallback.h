#pragma once

#include "GlobalOpenGL.h"

enum EditMode {
	MOVING_CAMERA,
	MOVING_LIGHT
};

EditMode editMode = MOVING_CAMERA;
unsigned int selectedLightIndex = 0;

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);