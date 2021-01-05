#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Triangle.h"

#define MAXPERLEAF 1 //Maximum number of objects in a leaf node

class kdTreeNode
{
	public:
		kdTreeNode* left=nullptr;
		kdTreeNode* right=nullptr;

		glm::vec3 AABBmax;
		glm::vec3 AABBmin;

		Triangle* LeafObject=nullptr;

		unsigned int VAO = 0;
		unsigned int VBO = 0;

		kdTreeNode(std::vector<Triangle*> Triangles);
		~kdTreeNode();

		void drawRecursively(void);

		enum Axis
		{
			x,
			y,
			z
		};

	private:
		void MergeAABB(void);

		Axis GetCuttingAxis(std::vector<Triangle*> Triangles);
		std::vector<Triangle*> SortTriangles(std::vector<Triangle*> triangles, Axis axis);
};

