#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>
#include <iterator>

#include "Triangle.h"

#define DIMENSIONS 3
#define RIGHT 0
#define LEFT 1
#define MIDDLE 2

class Ray
{
public:
	Ray() = default;
	Ray(glm::vec3 origin, glm::vec3 direction);
	~Ray() = default;
	bool tfhBoundingBox(glm::vec3 minimalBB, glm::vec3 maximalBB);
	short int tfhTriangle(Triangle triangle, float& tReturn);
	void Draw();

	//Ray
	float origin[DIMENSIONS], direction[DIMENSIONS];

private:
	unsigned int VAO = 0;
	unsigned int VBO = 0;

	

	//Hit Point
	float coordinates[DIMENSIONS];

	glm::vec3 crossProduct(const glm::vec3& vector1, const glm::vec3& vector2);
	float dotProduct(const glm::vec3& vector1, const glm::vec3& vector2);
};

