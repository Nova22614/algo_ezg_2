#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Triangle.h"
#include "Ray.h"

#define MAXPERLEAF 6 //Maximum number of objects in a leaf node

class kdTreeNode
{
	public:
		enum Axis
		{
			x,
			y,
			z
		};

		kdTreeNode* left=nullptr;
		kdTreeNode* right=nullptr;

		glm::vec3 AABBmax;
		glm::vec3 AABBmin;

		glm::vec3 AABBcenter;
		glm::vec3 AABBhalfLength;

		Axis separationAxis;
		float separationValue;

		//Triangle* LeafObject=nullptr;
		std::vector<Triangle*> LeafObjects;

		unsigned int VAO = 0;
		unsigned int VBO = 0;

		//kdTreeNode(std::vector<Triangle*> Triangles);
		kdTreeNode(glm::vec3 min, glm::vec3 max) : AABBmax(max), AABBmin(min) { CornerAABBtoCenterAABB(); }
		kdTreeNode() {}
		~kdTreeNode();

		void buildKDT(std::vector<Triangle*> Triangles);
		void buildBVH(std::vector<Triangle*> Triangles);

		void drawRecursively(void);
		std::pair<Triangle*, float> checkBVHForCollisionRecursively(Ray r);
		std::pair<Triangle*, float> checkKDTForCollisionRecursively(Ray r);



	private:
		void MergeAABB(void);
		void CornerAABBtoCenterAABB(void);

		Axis GetCuttingAxis(std::vector<Triangle*> Triangles);
		std::vector<Triangle*> SortTriangles(std::vector<Triangle*> triangles, Axis axis);
};

