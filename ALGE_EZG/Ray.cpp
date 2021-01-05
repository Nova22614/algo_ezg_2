#include "Ray.h"

Ray::Ray(glm::vec3 origin, glm::vec3 direction)
{
	this->origin[0] = origin.x;
	this->origin[1] = origin.y;
	this->origin[2] = origin.z;

	this->direction[0] = direction.x;
	this->direction[1] = direction.y;
	this->direction[2] = direction.z;
}

bool Ray::tfhBoundingBox(glm::vec3 minBB, glm::vec3 maxBB)
{
	float minimalBB[DIMENSIONS] = { minBB.x, minBB.y, minBB.z };
	float maximalBB[DIMENSIONS] = { maxBB.x, maxBB.y, maxBB.z };
	bool inside = true;
	int quadrant[DIMENSIONS];
	int plane;
	float maximalTDistance[DIMENSIONS];
	float candidatePlane[DIMENSIONS];

	for (int i = 0; i < DIMENSIONS; i++)
	{
		if (origin[i] < minimalBB[i])
		{
			quadrant[i] = LEFT;
			candidatePlane[i] = minimalBB[i];
			inside = false;
		}
		else if (origin[i] > maximalBB[i])
		{
			quadrant[i] = RIGHT;
			candidatePlane[i] = maximalBB[i];
			inside = false;
		}
		else
		{
			quadrant[i] = MIDDLE;
		}

	}

	if (inside)
	{
		std::copy(std::begin(origin), std::end(origin), std::begin(coordinates));
		return true;
	}

	for (int i = 0; i < DIMENSIONS; i++)
	{
		if (quadrant[i] != MIDDLE && direction[i] != 0)
		{
			maximalTDistance[i] = (candidatePlane[i] - origin[i]) / direction[i];
		}
		else
		{
			maximalTDistance[i] = -1.0f;
		}
	}

	plane = 0;
	for (int i = 1; i < DIMENSIONS; i++)
	{
		if (maximalTDistance[plane] < maximalTDistance[i])
		{
			plane = i;
		}
	}

	if (maximalTDistance[plane] < 0.0f)
	{
		return false;
	}

	for (int i = 0; i < DIMENSIONS; i++)
	{
		if (plane != i)
		{
			coordinates[i] = origin[i] + maximalTDistance[plane] * direction[i];
			if (coordinates[i] < minimalBB[i] || coordinates[i] > maximalBB[i])
			{
				return false;
			}
		}
		else
		{
			coordinates[i] = candidatePlane[i];
		}
	}

	return true;
}

short int Ray::tfhTriangle(Triangle triangle, float &tReturn)
{
	double eps = 10E-8;

	glm::vec3 pointA = triangle.points[0];
	glm::vec3 pointB = triangle.points[1];
	glm::vec3 pointC = triangle.points[2];

	glm::vec3 edge1 = pointB - pointA;
	glm::vec3 edge2 = pointC - pointA;

	glm::vec3 directionVector = glm::vec3(direction[0], direction[1], direction[2]);
	glm::vec3 originVector = glm::vec3(origin[0], origin[1], origin[2]);

	glm::vec3 h = crossProduct(directionVector, edge2);
	float a = dotProduct(edge1, h);

	if (a > -eps && a < eps)
	{
		return false;
	}

	float f = 1.0 / a;
	glm::vec3 s = originVector - pointA;
	float u = f * dotProduct(s, h);

	if (u < 0.0f || u > 1.0f)
	{
		return false;
	}

	glm::vec3 q = crossProduct(s, edge1);
	float v = f * dotProduct(directionVector, q);

	if (v < 0.0f || u + v > 1.0f)
	{
		return false;
	}

	float t = f * dotProduct(edge2, q);
	tReturn = t;

	if (t > eps)
	{
		return true;
	}
	else
	{
		return false;
	}
}

glm::vec3 Ray::crossProduct(const glm::vec3& vector1, const glm::vec3& vector2)
{
	return glm::vec3(
		vector1[1] * vector2[2] - vector1[2] * vector2[1],
		vector1[2] * vector2[0] - vector1[0] * vector2[2],
		vector1[0] * vector2[1] - vector1[1] * vector2[0]
	);
}

float Ray::dotProduct(const glm::vec3& vector1, const glm::vec3& vector2)
{
	return ((vector1[0] * vector2[0]) + (vector1[1] * vector2[1]) + (vector1[2] * vector2[2]));
}
