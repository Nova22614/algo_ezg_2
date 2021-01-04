#include "Triangle.h"
#include <glad/glad.h>

Triangle::Triangle(glm::vec3 A, glm::vec3 B, glm::vec3 C)
{
	points[0] = A;
	points[1] = B;
	points[2] = C;

	center = (A + B + C) / 3.0f;
}

glm::vec3 Triangle::CalcAABBmin(void)
{
	glm::vec3 res = points[0];
	for (int i = 1; i < 3; i++)
	{
		res.x = std::fminf(res.x, points[i].x);
		res.y = std::fminf(res.y, points[i].y);
		res.z = std::fminf(res.z, points[i].z);
	}
	return res;
}

glm::vec3 Triangle::CalcAABBmax(void)
{
	glm::vec3 res = points[0];
	for (int i = 1; i < 3; i++)
	{
		res.x = std::fmaxf(res.x, points[i].x);
		res.y = std::fmaxf(res.y, points[i].y);
		res.z = std::fmaxf(res.z, points[i].z);
	}
	return res;
}

void Triangle::Draw()
{
	if (VAO == 0)
	{
		float vertices[] = {
		points[0].x, points[0].y, points[0].z,
		points[1].x, points[1].y, points[1].z,
		points[2].x, points[2].y, points[2].z
		};


		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindVertexArray(VAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}
