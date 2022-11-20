#pragma once
#include <glm/glm.hpp>

struct BoundingBox {
	glm::vec3 centerPoint;
	glm::vec3 minPoints;
	glm::vec3 maxPoints;
	glm::vec3 halfExtents;
};