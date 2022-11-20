#include "MeshObject.h"

MeshObject::MeshObject(): 
	scale(1.0f), 
	isWireframe(false), 
	bUseRGBAColour(false), 
	bDoNotLight(false), 
	meshName(meshName), 
	friendlyName(friendlyName), 
	position(0.0f), 
	rotation(0.0f), 
	RGBAColour(0.0f, 0.0f, 0.0f, 1.0f)
{
}

MeshObject::~MeshObject()
{
}