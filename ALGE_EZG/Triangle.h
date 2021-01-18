#pragma once
#include <glm/glm.hpp>

struct Triangle
{
	glm::vec3 points[3];
	glm::vec3 center;
	unsigned int VBO=0, VAO=0;

	Triangle(glm::vec3 A, glm::vec3 B, glm::vec3 C);

	bool triBoxOverlap(glm::vec3 boxcenter, glm::vec3 boxhalfsize);

	glm::vec3 CalcAABBmin(void);
	glm::vec3 CalcAABBmax(void);
	void Draw();

	private:
	bool planeBoxOverlap(glm::vec3 normal, glm::vec3 vert, glm::vec3 maxbox);

	void findMinMax(float x0, float x1, float x2, float& min, float& max);

	bool axisTestX01(float a, float b, float fa, float fb, const glm::vec3& v0, const glm::vec3& v2, const glm::vec3& boxhalfsize, float& rad, float& min, float& max, float& p0, float& p2);

	bool axisTestX2(float a, float b, float fa, float fb, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& boxhalfsize, float& rad, float& min, float& max, float& p0, float& p1);

	bool axisTestY02(float a, float b, float fa, float fb, const glm::vec3& v0, const glm::vec3& v2, const glm::vec3& boxhalfsize, float& rad, float& min, float& max, float& p0, float& p2);

	bool axisTestY1(float a, float b, float fa, float fb, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& boxhalfsize, float& rad, float& min, float& max, float& p0, float& p1);

	bool axisTestZ12(float a, float b, float fa, float fb, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& boxhalfsize, float& rad, float& min, float& max, float& p1, float& p2);

	bool axisTestZ0(float a, float b, float fa, float fb, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& boxhalfsize, float& rad, float& min, float& max, float& p0, float& p1);

};

