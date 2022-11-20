#pragma once

#include <glm/glm.hpp>

enum LightType
{
	POINT_LIGHT,
	DIRECTIONAL_LIGHT,
	SPOT_LIGHT
};

class Light
{
public:
	Light();
	~Light();
	int positionUniformLocation;
	int diffuseUniformLocation;
	int specularUniformLocation;
	int attenUniformLocation;
	int directionUniformLocation;
	int param1UniformLocation;
	int param2UniformLocation;

	glm::vec4 position;
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::vec4 atten;
	glm::vec4 direction;
	glm::vec4 param1;
	glm::vec4 param2;

	void TurnOn();
	void TurnOff();
	void SetToWhite();
	void SetToBlack();
	void SetLightType(LightType newLightType);
	void MakeItAPointLight();
	void MakeItASpotLight(glm::vec3 direction, float innerAngle, float outerAngle);
	void SetConstantAttenuation(float newConstAtten);
	void SetLinearAttenuation(float newLinearAtten);
	void SetQuadraticAttenuation(float newQuadAtten);
};
