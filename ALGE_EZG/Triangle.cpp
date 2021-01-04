#include "Triangle.h"

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