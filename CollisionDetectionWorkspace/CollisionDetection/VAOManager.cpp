#include "VAOManager.h"

ModelDrawInfo::ModelDrawInfo() {

	this->VAOId = 0;

	this->VertexBufferId = 0;
	this->VertexBufferStartIndex = 0;
	this->numberOfVertices = 0;

	this->IndexBufferId = 0;
	this->IndexBufferStartIndex = 0;
	this->numberOfIndices = 0;
	this->numberOfTriangles = 0;

	this->pVertices = 0;
	this->pTriangles = 0;
	this->pIndices = 0;

	glm::vec3 maxValues;
	glm::vec3 minValues;

	float maxExtent;

	return;
}

VAOManager::VAOManager()
{
}

VAOManager::~VAOManager()
{
}

bool VAOManager::LoadModelIntoVAO(
	std::string fileName, 
	ModelDrawInfo& drawInfo, 
	unsigned int shaderProgramID)
{
	drawInfo.meshName = fileName;

	glGenVertexArrays(1, &(drawInfo.VAOId));
	glBindVertexArray(drawInfo.VAOId);

	glGenBuffers(1, &(drawInfo.VertexBufferId));
	glBindBuffer(GL_ARRAY_BUFFER, drawInfo.VertexBufferId);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(Vertex) * drawInfo.numberOfVertices, 
		(GLvoid*)drawInfo.pVertices,
		GL_STATIC_DRAW);

	glGenBuffers(1, &(drawInfo.IndexBufferId));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawInfo.IndexBufferId);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(unsigned int) * drawInfo.numberOfIndices,
		(GLvoid*)drawInfo.pIndices,
		GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(unsigned int) * drawInfo.numberOfTriangles,
		(GLvoid*)drawInfo.pTriangles,
		GL_STATIC_DRAW);

	GLint vpos_location = glGetAttribLocation(shaderProgramID, "vPos");
	GLint vcol_location = glGetAttribLocation(shaderProgramID, "vCol");
	GLint vNormal_location = glGetAttribLocation(shaderProgramID, "vNormal");

	glEnableVertexAttribArray(vpos_location);
	glVertexAttribPointer(vpos_location, 3,
		GL_FLOAT, GL_FALSE,
		sizeof(Vertex),
		(void*)offsetof(Vertex, x));

	glEnableVertexAttribArray(vcol_location);
	glVertexAttribPointer(vcol_location, 3,
		GL_FLOAT, GL_FALSE,
		sizeof(Vertex),
		(void*)offsetof(Vertex, r));

	glEnableVertexAttribArray(vNormal_location);
	glVertexAttribPointer(vNormal_location, 3,
		GL_FLOAT, GL_FALSE,
		sizeof(Vertex),
		(void*)offsetof(Vertex, nx));

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(vpos_location);
	glDisableVertexAttribArray(vcol_location);
	glDisableVertexAttribArray(vNormal_location);

	this->mMapModelNameToVAOId[drawInfo.meshName] = drawInfo;

	return true;
}

bool VAOManager::FindDrawInfoByModelName(std::string filename, ModelDrawInfo& drawInfo)
{
	std::map<std::string, ModelDrawInfo>::iterator itDrawInfo = 
		this->mMapModelNameToVAOId.find(filename);

	if (itDrawInfo == this->mMapModelNameToVAOId.end())
		return false;

	drawInfo = itDrawInfo->second;
	return true;
}

std::string VAOManager::GetLastError(bool bAndClear)
{
	return std::string();
}
