#pragma once

#include <memory>
#include <glm/glm.hpp>

enum ShapeType {
	SHAPE_TYPE_SPHERE,
	SHAPE_TYPE_AABB,
	SHAPE_TYPE_TRIANGLE
};

class iShape {
public:
	virtual ShapeType GetType() const = 0;
};

class Sphere : public iShape {
public:
	Sphere(const glm::vec3& center, float radius)
		: Center(center)
		, Radius(radius)
	{ }

	virtual ~Sphere()
	{ }

	virtual ShapeType GetType() const override {
		return SHAPE_TYPE_SPHERE;
	}

	glm::vec3 Center;
	float Radius;
};

class AABB : public iShape {
public:
	AABB(float min[3], float max[3]) {
		memcpy(&(Min[0]), &(min[0]), 3 * sizeof(float));
		memcpy(&(Max[0]), &(max[0]), 3 * sizeof(float));
	}
	virtual ~AABB() {
	}

	virtual ShapeType GetType() const override {
		return SHAPE_TYPE_AABB;
	}

	// x, y, z will be indexed as 0, 1, 2 respectively
	float Min[3];
	float Max[3];
};

class Triangle : public iShape {
public:
	Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c)
		: A(a), B(b), C(c) { }
	virtual ~Triangle() { }

	virtual ShapeType GetType() const override {
		return SHAPE_TYPE_TRIANGLE;
	}

	glm::vec3 A;
	glm::vec3 B;
	glm::vec3 C;
};