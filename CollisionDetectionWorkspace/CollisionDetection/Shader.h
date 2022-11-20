#pragma once
#include <vector>
#include <string>
#include "GlobalOpenGL.h"

enum ShaderType
{
	VERTEX_SHADER,
	FRAGMENT_SHADER,
	UNKNOWN
};

class Shader
{
public:
	Shader();
	~Shader();
	unsigned int id;
	std::vector<std::string> vecSource;
	bool bSourceIsMultiLine;
	std::string fileName;
	ShaderType shaderType;

	std::string getShaderTypeString();

private:

};