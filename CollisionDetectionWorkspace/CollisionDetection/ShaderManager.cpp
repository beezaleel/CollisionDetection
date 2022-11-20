#include "ShaderManager.h"

const unsigned int MAXLINELENGTH = 65536;

ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
}

bool ShaderManager::UseShaderProgram(unsigned int id)
{
	glUseProgram(id);
	return true;
}

bool ShaderManager::UseShaderProgram(std::string friendlyName)
{
	std::map<std::string, unsigned int>::iterator itShad = this->mNameToId.find(friendlyName);

	if (itShad == this->mNameToId.end())
	{	
		return false;
	}
	glUseProgram(itShad->second);

	return true;
}

bool ShaderManager::CreateProgramFromFile(std::string friendlyName, Shader& vertexShad, Shader& fragShader)
{
	std::string errorText = "";


	vertexShad.id = glCreateShader(GL_VERTEX_SHADER);
	vertexShad.shaderType = ShaderType::VERTEX_SHADER;

	if (!this->mLoadSourceFromFile(vertexShad))
	{
		return false;
	}

	errorText = "";
	if (!this->mCompileShaderFromSource(vertexShad, errorText))
	{
		this->mLastError = errorText;
		return false;
	}

	fragShader.id = glCreateShader(GL_FRAGMENT_SHADER);
	fragShader.shaderType = ShaderType::FRAGMENT_SHADER;
	if (!this->mLoadSourceFromFile(fragShader))
	{
		return false;
	}

	if (!this->mCompileShaderFromSource(fragShader, errorText))
	{
		this->mLastError = errorText;
		return false;
	}

	ShaderProgram curProgram;
	curProgram.id = glCreateProgram();

	glAttachShader(curProgram.id, vertexShad.id);
	glAttachShader(curProgram.id, fragShader.id);
	glLinkProgram(curProgram.id);

	errorText = "";
	if (this->mWasThereALinkError(curProgram.id, errorText))
	{
		std::stringstream ssError;
		ssError << "Shader program link error: ";
		ssError << errorText;
		this->mLastError = ssError.str();
		return false;
	}

	curProgram.friendlyName = friendlyName;

	this->mIdToShader[curProgram.id] = curProgram;
	this->mNameToId[curProgram.friendlyName] = curProgram.id;

	return true;
}

void ShaderManager::SetBasePath(std::string basepath)
{
	this->mBasepath = basepath;
	return;
}

unsigned int ShaderManager::GetIDFromFriendlyName(std::string friendlyName)
{
	std::map<std::string, unsigned int>::iterator itShad = this->mNameToId.find(friendlyName);

	if (itShad == this->mNameToId.end())
	{
		return 0;
	}

	return itShad->second;
}

ShaderProgram* ShaderManager::PGetShaderProgramFromFriendlyName(std::string friendlyName)
{
	unsigned int shaderID = this->GetIDFromFriendlyName(friendlyName);

	std::map< unsigned int, ShaderProgram >::iterator itShad = this->mIdToShader.find(shaderID);

	if (itShad == this->mIdToShader.end())
	{
		return NULL;
	}

	ShaderProgram* pShaderIFound = &(itShad->second);

	return pShaderIFound;
}

std::string ShaderManager::GetLastError()
{
	std::string lastErrorTemp = this->mLastError;
	this->mLastError = "";
	return lastErrorTemp;
}

bool ShaderManager::mLoadSourceFromFile(Shader& shader)
{
	std::string fullFileName = this->mBasepath + shader.fileName;

	std::ifstream theFile(fullFileName.c_str());
	if (!theFile.is_open())
	{
		return false;
	}

	shader.vecSource.clear();

	char pLineTemp[MAXLINELENGTH] = { 0 };
	while (theFile.getline(pLineTemp, MAXLINELENGTH))
	{
		std::string tempString(pLineTemp);
		shader.vecSource.push_back(tempString);
	}

	theFile.close();
	return true;
}

bool ShaderManager::mCompileShaderFromSource(Shader& shader, std::string& error)
{
	error = "";

	const unsigned int MAXLINESIZE = 8 * 1024;
	unsigned int numberOfLines = static_cast<unsigned int>(shader.vecSource.size());
	char** arraySource = new char* [numberOfLines];

	memset(arraySource, 0, numberOfLines);

	for (unsigned int indexLine = 0; indexLine != numberOfLines; indexLine++)
	{
		unsigned int numCharacters = (unsigned int)shader.vecSource[indexLine].length();
		arraySource[indexLine] = new char[numCharacters + 2];
		memset(arraySource[indexLine], 0, numCharacters + 2);

		for (unsigned int indexChar = 0; indexChar != shader.vecSource[indexLine].length(); indexChar++)
		{
			arraySource[indexLine][indexChar] = shader.vecSource[indexLine][indexChar];
		}

		arraySource[indexLine][numCharacters + 0] = '\n';
		arraySource[indexLine][numCharacters + 1] = '\0';
	}
		glShaderSource(shader.id, numberOfLines, arraySource, NULL);
		glCompileShader(shader.id);

		for (unsigned int indexLine = 0; indexLine != numberOfLines; indexLine++)
		{
			delete[] arraySource[indexLine];
		}

		delete[] arraySource;

		std::string errorText = "";
		if (this->mWasThereACompileError(shader.id, errorText))
		{
			std::stringstream ssError;
			ssError << shader.getShaderTypeString();
			ssError << " compile error: ";
			ssError << errorText;
			error = ssError.str();
			return false;
		}

		return true;
	}

bool ShaderManager::mWasThereACompileError(unsigned int shaderID, std::string& errorText)
{
	errorText = "";

	GLint isCompiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

		char* pLogText = new char[maxLength];

		glGetShaderInfoLog(shaderID, maxLength, &maxLength, pLogText);

		errorText.append(pLogText);

		this->mLastError.append("\n");
		this->mLastError.append(errorText);


		delete[] pLogText;

		return true;
	}
	return false;
}

bool ShaderManager::mWasThereALinkError(unsigned int programId, std::string& errorText)
{
	errorText = "";	

	GLint wasError = 0;
	glGetProgramiv(programId, GL_LINK_STATUS, &wasError);
	if (wasError == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &maxLength);

		char* pLogText = new char[maxLength];
		glGetProgramInfoLog(programId, maxLength, &maxLength, pLogText);

		errorText.append(pLogText);

		this->mLastError.append("\n");
		this->mLastError.append(errorText);

		delete[] pLogText;

		return true;
	}
	return false;
}
