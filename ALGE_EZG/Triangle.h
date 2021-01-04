#pragma once
#include <glm/glm.hpp>

struct Triangle
{
	glm::vec3 points[3];
	glm::vec3 center;
	unsigned int VBO=0, VAO=0;

	Triangle(glm::vec3 A, glm::vec3 B, glm::vec3 C);

	glm::vec3 CalcAABBmin(void);
	glm::vec3 CalcAABBmax(void);
	void Draw();

};

