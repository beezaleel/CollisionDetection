#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem()
{
}

PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::AddTriangleToAABBCollisionCheck(int hash, Triangle* triangle)
{
	mAABBStructure[hash].push_back(triangle);
}

std::map<int, std::vector<Triangle*>> PhysicsSystem::GetAABBStructure()
{
	return mAABBStructure;
}
