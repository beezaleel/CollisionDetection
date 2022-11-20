#pragma once

#include <string>
#include <glad/glad.h>
#include <vector>
#include "Light.h"

static const unsigned int NUMBER_OF_LIGHTS = 2;

class LightManager
{
public:
	LightManager();
	~LightManager();
	std::vector<Light> vLights;

	void LoadLightUniformLocations(unsigned int shaderID);
	void CopyLightInformationToShader(unsigned int shaderID);

private:

};
