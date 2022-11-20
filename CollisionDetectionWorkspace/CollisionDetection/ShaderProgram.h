#pragma once

#include <glad/glad.h>
#include <string>
#include <map>

class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();
	unsigned int id;
	std::string friendlyName;
	std::map<std::string, int> mapUniformNameToUniformLocation;

	int GetUniformIdFromName(std::string name);
	bool LoadUniformLocation(std::string variableName);
};
