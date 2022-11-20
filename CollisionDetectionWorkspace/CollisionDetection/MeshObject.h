#pragma once

#include <string>
#include <glm/glm.hpp>

class MeshObject
{
public:
	MeshObject();
	~MeshObject();
	float scale;

	bool isWireframe;
	bool bUseRGBAColour;
	bool bDoNotLight;

	std::string meshName;
	std::string friendlyName;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec4 RGBAColour;
};
