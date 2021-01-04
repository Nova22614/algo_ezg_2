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

bool Ray::tfhBoundingBox(glm::vec3 minimalBB, glm::vec3 maximalBB)
{
	float minimalBB[DIMENSIONS] = { minimalBB.x, minimalBB.y, minimalBB.z };
	float maximalBB[DIMENSIONS] = { maximalBB.x, maximalBB.y, maximalBB.z };
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
			plane = 1;
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
