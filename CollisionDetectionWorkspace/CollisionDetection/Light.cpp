#include "Light.h"

Light::Light(): 
	positionUniformLocation(-1), 
	diffuseUniformLocation(-1), 
	specularUniformLocation(-1), 
	attenUniformLocation(-1), 
	directionUniformLocation(-1), 
	param1UniformLocation(-1), 
	param2UniformLocation(-1),
	position(0.0f, 0.0f, 0.0f, 1.0f), 
	diffuse(1.0f, 1.0f, 1.0f, 1.0f), 
	specular(1.0f, 1.0f, 1.0f, 1.0f), 
	atten(0.01f, 0.01f, 0.0f, 1.0f), 
	direction(0.0f, 0.0f, 1.0f, 1.0f), 
	param1(0.0f, 0.0f, 0.0f, 1.0f), 
	param2(0.0f, 0.0f, 0.0f, 1.0f)

{
}

Light::~Light()
{
}

void Light::TurnOn() {
	this->param2.x = 1;
}

void Light::TurnOff() {
	this->param2.x = 0;
}

void Light::SetToWhite() {

}

void Light::SetToBlack() {

}

void Light::SetLightType(LightType newLightType) {

}

void Light::MakeItAPointLight() {

}

void Light::MakeItASpotLight(glm::vec3 direction, float innerAngle, float outerAngle) {

}

void Light::SetConstantAttenuation(float newConstAtten) {
	this->atten.x = newConstAtten;
}

void Light::SetLinearAttenuation(float newLinearAtten) {
	this->atten.y = newLinearAtten;
}

void Light::SetQuadraticAttenuation(float newQuadAtten) {
	this->atten.z = newQuadAtten;
}
