#include "ShaderProgram.h"

ShaderProgram::ShaderProgram() : id(0)
{
}

ShaderProgram::~ShaderProgram()
{
}

int ShaderProgram::GetUniformIdFromName(std::string name)
{
	std::map<std::string, int >::iterator itUniform = this->mapUniformNameToUniformLocation.find(name);

	if (itUniform == this->mapUniformNameToUniformLocation.end())
	{
		return -1;
	}

	return itUniform->second;
}

bool ShaderProgram::LoadUniformLocation(std::string variableName)
{
	GLint uniLocation = glGetUniformLocation(this->id, variableName.c_str());

	if (uniLocation == -1)
	{
		return false;
	}
	
	this->mapUniformNameToUniformLocation[variableName.c_str()] = uniLocation;

	return true;
}
