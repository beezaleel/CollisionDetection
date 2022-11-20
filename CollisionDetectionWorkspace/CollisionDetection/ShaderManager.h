#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include "Shader.h"
#include "ShaderProgram.h"

class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();

	bool UseShaderProgram(unsigned int id);
	bool UseShaderProgram(std::string friendlyName);
	bool CreateProgramFromFile(std::string friendlyName, Shader& vertexShad, Shader& fragShader);
	void SetBasePath(std::string basepath);
	unsigned int GetIDFromFriendlyName(std::string friendlyName);

	ShaderProgram* PGetShaderProgramFromFriendlyName(std::string friendlyName);
	std::string GetLastError();
private:
	bool mLoadSourceFromFile(Shader& shader);
	std::string mBasepath;

	bool mCompileShaderFromSource(Shader& shader, std::string& error);

	bool mWasThereACompileError(unsigned int shaderID, std::string& errorText);
	bool mWasThereALinkError(unsigned int programId, std::string& errorText);

	std::string mLastError;

	std::map<unsigned int, ShaderProgram > mIdToShader;
	std::map<std::string, unsigned int> mNameToId;
};
