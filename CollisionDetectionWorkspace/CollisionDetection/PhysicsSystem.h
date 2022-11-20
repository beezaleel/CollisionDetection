#pragma once
#include "Shapes.h"
#include <map>
#include <vector>

class PhysicsSystem
{
public:
	PhysicsSystem();
	~PhysicsSystem();
	void AddTriangleToAABBCollisionCheck(int hash, Triangle* triangle);
	std::map<int, std::vector<Triangle*>> GetAABBStructure();

private:
	std::map<int, std::vector<Triangle*>> mAABBStructure;
};
