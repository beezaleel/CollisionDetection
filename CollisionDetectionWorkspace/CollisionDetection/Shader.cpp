#include "Shader.h"

Shader::Shader() : 
	id(0), 
	shaderType(ShaderType::UNKNOWN)
{
}

Shader::~Shader()
{
}

std::string Shader::getShaderTypeString()
{
	switch (this->shaderType)
	{
	case ShaderType::VERTEX_SHADER:
		return "VERTEX_SHADER";
		break;
	case ShaderType::FRAGMENT_SHADER:
		return "FRAGMENT_SHADER";
		break;
	case ShaderType::UNKNOWN:
	default:
		return "UNKNOWN_SHADER_TYPE";
		break;
	}

	return "UNKNOWN_SHADER_TYPE";
}
