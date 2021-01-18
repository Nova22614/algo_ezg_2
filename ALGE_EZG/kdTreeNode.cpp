#include "kdTreeNode.h"

//kdTreeNode::kdTreeNode(std::vector<Triangle*> Triangles)
//{
//    if(Triangles.size() <= MAXPERLEAF)
//    {        
//        //TODO: implement possibility for multiple triangles per leaf
//        //save current triangle
//        LeafObject = Triangles.front();
//
//        //set correct AABB
//        AABBmax = LeafObject->CalcAABBmax();
//        AABBmin = LeafObject->CalcAABBmin();
//
//        //jump back
//        return;
//    }
//    else
//    {
//        //find longest axis
//        Axis splittingAxis = GetCuttingAxis(Triangles);
//
//        //sort triangles centers on found axis
//        Triangles = SortTriangles(Triangles, splittingAxis);
//
//        //split triangles in two lists and create child nodes to handle them
//        std::size_t const half_size = Triangles.size() / 2;
//        left = new kdTreeNode(std::vector<Triangle*>(Triangles.begin(), Triangles.begin() + half_size));
//        right = new kdTreeNode(std::vector<Triangle*>(Triangles.begin() + half_size, Triangles.end()));
//
//        //set correct AABB
//        MergeAABB();
//    }
//}

kdTreeNode::~kdTreeNode()
{
    delete right;
    delete left;
}

void kdTreeNode::buildKDT(std::vector<Triangle*> Triangles)
{
    if (Triangles.size() <= MAXPERLEAF)
    {
        //save current triangles
        LeafObjects = Triangles;

        //jump back
        return;
    }
    else
    {
        //find longest axis
        separationAxis = GetCuttingAxis(Triangles);

        //sort triangles centers on found axis
        Triangles = SortTriangles(Triangles, separationAxis);
        if (Triangles.size() % 2 == 0)
        {
            separationValue = (Triangles[((int)Triangles.size() / 2) - 1]->center[separationAxis] + Triangles[((int)Triangles.size() / 2)]->center[separationAxis])/2;
        }
        else
        {
            separationValue = Triangles[((int)Triangles.size() / 2)]->center[separationAxis];
        }

        std::vector<Triangle*> leftList, rightList;
        glm::vec3 tempMax = AABBmax;
        tempMax[separationAxis] = separationValue;
        glm::vec3 tempMin = AABBmin;
        tempMin[separationAxis] = separationValue;

        left = new kdTreeNode(AABBmin, tempMax);
        right = new kdTreeNode(tempMin, AABBmax);

        //split triangles in two lists and create child nodes to handle them
        for (int i = 0; i < Triangles.size(); i++)
        {
            if (Triangles[i]->triBoxOverlap(left->AABBcenter, left->AABBhalfLength))
            {
                leftList.push_back(Triangles[i]);
            }
            if (Triangles[i]->triBoxOverlap(right->AABBcenter, right->AABBhalfLength))
            {
                rightList.push_back(Triangles[i]);
            }
        }

        left->buildKDT(leftList);
        right->buildKDT(rightList);
    }
}

void kdTreeNode::buildBVH(std::vector<Triangle*> Triangles)
{
    if (Triangles.size() <= MAXPERLEAF)
    {
        
        //save current triangle
        //LeafObject = Triangles.front();
        LeafObjects = Triangles;

        //set correct AABB
        AABBmax = LeafObjects.front()->CalcAABBmax();
        AABBmin = LeafObjects.front()->CalcAABBmin();

        for (int i = 1; i < LeafObjects.size(); i++)
        {
            glm::vec3 temp = LeafObjects[i]->CalcAABBmax();

            AABBmax.x = std::fmaxf(AABBmax.x, right->AABBmax.x);
            AABBmax.y = std::fmaxf(AABBmax.y, right->AABBmax.y);
            AABBmax.z = std::fmaxf(AABBmax.z, right->AABBmax.z);

            temp = LeafObjects[i]->CalcAABBmin();

            AABBmin.x = std::fminf(AABBmin.x, right->AABBmin.x);
            AABBmin.y = std::fminf(AABBmin.y, right->AABBmin.y);
            AABBmin.z = std::fminf(AABBmin.z, right->AABBmin.z);
        }

        //jump back
        return;
    }
    else
    {
        //find longest axis
        Axis splittingAxis = GetCuttingAxis(Triangles);

        //sort triangles centers on found axis
        Triangles = SortTriangles(Triangles, splittingAxis);

        //split triangles in two lists and create child nodes to handle them
        std::size_t const half_size = Triangles.size() / 2;
        left = new kdTreeNode();
        right = new kdTreeNode();

        left->buildBVH(std::vector<Triangle*>(Triangles.begin(), Triangles.begin() + half_size));
        right->buildBVH(std::vector<Triangle*>(Triangles.begin() + half_size, Triangles.end()));

        //set correct AABB
        MergeAABB();
    }
}

void kdTreeNode::drawRecursively(void)
{
    if (VAO == 0)
    {
        // Make line
        float vertices[] = {
            AABBmax.x, AABBmax.y, AABBmax.z,
            AABBmin.x, AABBmax.y, AABBmax.z,

            AABBmin.x, AABBmax.y, AABBmax.z,
            AABBmin.x, AABBmax.y, AABBmin.z,

            AABBmin.x, AABBmax.y, AABBmin.z,
            AABBmax.x, AABBmax.y, AABBmin.z,

            AABBmax.x, AABBmax.y, AABBmin.z,
            AABBmax.x, AABBmax.y, AABBmax.z,


            AABBmax.x, AABBmax.y, AABBmax.z,
            AABBmax.x, AABBmin.y, AABBmax.z,

            AABBmin.x, AABBmax.y, AABBmax.z,
            AABBmin.x, AABBmin.y, AABBmax.z,

            AABBmin.x, AABBmax.y, AABBmin.z,
            AABBmin.x, AABBmin.y, AABBmin.z,

            AABBmax.x, AABBmax.y, AABBmin.z,
            AABBmax.x, AABBmin.y, AABBmin.z,


            AABBmax.x, AABBmin.y, AABBmax.z,
            AABBmin.x, AABBmin.y, AABBmax.z,

            AABBmin.x, AABBmin.y, AABBmax.z,
            AABBmin.x, AABBmin.y, AABBmin.z,

            AABBmin.x, AABBmin.y, AABBmin.z,
            AABBmax.x, AABBmin.y, AABBmin.z,

            AABBmax.x, AABBmin.y, AABBmin.z,
            AABBmax.x, AABBmin.y, AABBmax.z
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
    glDrawArrays(GL_LINES, 0, 24);
    glBindVertexArray(0);

    //draw child nodes
    if (left != nullptr)
    {
       left->drawRecursively();
    }
    if (right != nullptr)
    {
       right->drawRecursively();
    }
}

std::pair<Triangle*, float> kdTreeNode::checkBVHForCollisionRecursively(Ray r)
{
    if (r.tfhBoundingBox(AABBmin, AABBmax))
    {
        if (LeafObjects.size() > 0)
        {
            std::pair<Triangle*, float> bestTemp(LeafObjects[0], -1);
            r.tfhTriangle(*LeafObjects[0], bestTemp.second);

            for (int i = 1; i < LeafObjects.size(); i++)
            {
                std::pair<Triangle*, float> temp(LeafObjects[i], -1);
                r.tfhTriangle(*LeafObjects[i], temp.second);

                if (temp.second >= 0 && (bestTemp.second < 0 || temp.second < bestTemp.second))
                {
                    bestTemp = temp;
                }
            }

            return bestTemp;
        }
        else
        {
            std::pair<Triangle*, float> templ = left->checkBVHForCollisionRecursively(r);
            std::pair<Triangle*, float> tempr = right->checkBVHForCollisionRecursively(r);

            if (tempr.second >= 0 && (templ.second < 0 || tempr.second < templ.second))
            {
                return tempr;
            }
            else
            {
                return templ;
            }
        }
    }
    else
    {
        return std::pair<Triangle*, float>(nullptr, -1);
    }
}

std::pair<Triangle*, float> kdTreeNode::checkKDTForCollisionRecursively(Ray r)
{
    if (LeafObjects.size() > 0)
    {
        std::pair<Triangle*, float> bestTemp(LeafObjects[0], -1);
        r.tfhTriangle(*LeafObjects[0], bestTemp.second);

        for (int i = 1; i < LeafObjects.size(); i++)
        {
            std::pair<Triangle*, float> temp(LeafObjects[i], -1);
            r.tfhTriangle(*LeafObjects[i], temp.second);

            if (temp.second >= 0 && (bestTemp.second < 0 || temp.second < bestTemp.second))
            {
                bestTemp = temp;
            }
        }

        return bestTemp;
    }
    else
    {
        // Find t value for intersection between segment and split plane
        float t = (separationValue - r.origin[separationAxis]) / r.direction[separationAxis];
        int first = r.origin[separationAxis] > separationAxis;
        // Test if line segment straddles splitting plane
        if (0.0f <= t) 
        {
            // Yes, traverse near side first, then far side
            if (first)
            { 
                std::pair<Triangle*, float> temp = right->checkKDTForCollisionRecursively(r);
                if (temp.second >= 0)
                {
                    return temp;
                }
                return left->checkKDTForCollisionRecursively(r);
            }
            else
            {
                std::pair<Triangle*, float> temp = left->checkKDTForCollisionRecursively(r);
                if (temp.second >= 0)
                {
                    return temp;
                }
                return right->checkKDTForCollisionRecursively(r);
            }
        }
        else 
        {
            // No, so just traverse near side
            if (first)
            {
                return right->checkKDTForCollisionRecursively(r);
            }
            else
            {
                return left->checkKDTForCollisionRecursively(r);
            }
        }
    }
}

void kdTreeNode::MergeAABB(void)
{
	AABBmax.x = std::fmaxf(left->AABBmax.x, right->AABBmax.x);
	AABBmax.y = std::fmaxf(left->AABBmax.y, right->AABBmax.y);
	AABBmax.z = std::fmaxf(left->AABBmax.z, right->AABBmax.z);

	AABBmin.x = std::fminf(left->AABBmin.x, right->AABBmin.x);
	AABBmin.y = std::fminf(left->AABBmin.y, right->AABBmin.y);
	AABBmin.z = std::fminf(left->AABBmin.z, right->AABBmin.z);
}

void kdTreeNode::CornerAABBtoCenterAABB(void)
{
    AABBcenter = AABBmin + (AABBmax - AABBmin) / 2.0f;
    AABBhalfLength = AABBmax - AABBcenter;
}

kdTreeNode::Axis kdTreeNode::GetCuttingAxis(std::vector<Triangle*> Triangles)
{
    float maxX = INT32_MIN, maxY = INT32_MIN, maxZ = INT32_MIN;
    float minX = INT32_MAX, minY = INT32_MAX, minZ = INT32_MAX;

    for (Triangle* triangle : Triangles)
    {
        maxX = std::fmaxf(maxX, triangle->center.x);
        maxY = std::fmaxf(maxY, triangle->center.y);
        maxZ = std::fmaxf(maxZ, triangle->center.z);
        minX = std::fminf(minX, triangle->center.x);
        minY = std::fminf(minY, triangle->center.y);
        minZ = std::fminf(minZ, triangle->center.z);
    }

    float difX = maxX - minX;
    float difY = maxY - minY;
    float difZ = maxZ - minZ;

    if (difX >= difY && difX >= difZ)
    {
        return Axis::x;
    }
    else if (difY >= difZ)
    {
        return Axis::y;
    }
    else
    {
        return Axis::z;
    }
}

std::vector<Triangle*> kdTreeNode::SortTriangles(std::vector<Triangle*> triangles, Axis axis)
{
    switch (axis)
    {
    case Axis::x:
        std::sort(triangles.begin(), triangles.end(), [](const Triangle* t1, const Triangle* t2) {
            return t1->center.x < t2->center.x;
            });
        break;
    case Axis::y:
        std::sort(triangles.begin(), triangles.end(), [](const Triangle* t1, const Triangle* t2) {
            return t1->center.y < t2->center.y;
            });
        break;
    case Axis::z:
        std::sort(triangles.begin(), triangles.end(), [](const Triangle* t1, const Triangle* t2) {
            return t1->center.z < t2->center.z;
            });
        break;
    }

    return triangles;
}