#include "LightManager.h"

LightManager::LightManager()
{
	for (unsigned int count = 0; count != NUMBER_OF_LIGHTS; count++) {
		Light newLight;
		this->vLights.push_back(newLight);
	}
}

LightManager::~LightManager()
{
}

void LightManager::LoadLightUniformLocations(unsigned int shaderID)
{
	this->vLights[0].positionUniformLocation = glGetUniformLocation(shaderID, "theLights[0].position");
	this->vLights[0].diffuseUniformLocation = glGetUniformLocation(shaderID, "theLights[0].diffuse");
	this->vLights[0].specularUniformLocation = glGetUniformLocation(shaderID, "theLights[0].specular");
	this->vLights[0].attenUniformLocation = glGetUniformLocation(shaderID, "theLights[0].atten");
	this->vLights[0].directionUniformLocation = glGetUniformLocation(shaderID, "theLights[0].direction");
	this->vLights[0].param1UniformLocation = glGetUniformLocation(shaderID, "theLights[0].param1");
	this->vLights[0].param2UniformLocation = glGetUniformLocation(shaderID, "theLights[0].param2");

	this->vLights[1].positionUniformLocation = glGetUniformLocation(shaderID, "theLights[1].position");
	this->vLights[1].diffuseUniformLocation = glGetUniformLocation(shaderID, "theLights[1].diffuse");
	this->vLights[1].specularUniformLocation = glGetUniformLocation(shaderID, "theLights[1].specular");
	this->vLights[1].attenUniformLocation = glGetUniformLocation(shaderID, "theLights[1].atten");
	this->vLights[1].directionUniformLocation = glGetUniformLocation(shaderID, "theLights[1].direction");
	this->vLights[1].param1UniformLocation = glGetUniformLocation(shaderID, "theLights[1].param1");
	this->vLights[1].param2UniformLocation = glGetUniformLocation(shaderID, "theLights[1].param2");
}

void LightManager::CopyLightInformationToShader(unsigned int shaderID)
{
	for (std::vector<Light>::iterator itLight = vLights.begin(); itLight != vLights.end(); itLight++) {
		glUniform4f(itLight->positionUniformLocation,
			itLight->position.x,
			itLight->position.y,
			itLight->position.z,
			itLight->position.w);

		glUniform4f(itLight->diffuseUniformLocation,
			itLight->diffuse.x,
			itLight->diffuse.y,
			itLight->diffuse.z,
			itLight->diffuse.w);

		glUniform4f(itLight->specularUniformLocation,
			itLight->specular.x,
			itLight->specular.y,
			itLight->specular.z,
			itLight->specular.w);

		glUniform4f(itLight->attenUniformLocation,
			itLight->atten.x,
			itLight->atten.y,
			itLight->atten.z,
			itLight->atten.w);

		glUniform4f(itLight->directionUniformLocation,
			itLight->direction.x,
			itLight->direction.y,
			itLight->direction.z,
			itLight->direction.w);

		glUniform4f(itLight->param1UniformLocation,
			itLight->param1.x,
			itLight->param1.y,
			itLight->param1.z,
			itLight->param1.w);

		glUniform4f(itLight->param2UniformLocation,
			itLight->param2.x,
			itLight->param2.y,
			itLight->param2.z,
			itLight->param2.w);
	}
}
