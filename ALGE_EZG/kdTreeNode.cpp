#include "kdTreeNode.h"

kdTreeNode::kdTreeNode(std::vector<Triangle*> Triangles)
{
    if (Triangles.size() <= MAXPERLEAF)
    {        
        //TODO: implement possibility for multiple triangles per leaf
        //save current triangle
        LeafObject = Triangles.front();

        //set correct AABB
        AABBmax = LeafObject->CalcAABBmax();
        AABBmin = LeafObject->CalcAABBmin();

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
        left = new kdTreeNode(std::vector<Triangle*>(Triangles.begin(), Triangles.begin() + half_size));
        right = new kdTreeNode(std::vector<Triangle*>(Triangles.begin() + half_size, Triangles.end()));

        //set correct AABB
        MergeAABB();
    }
}

kdTreeNode::~kdTreeNode()
{
    delete right;
    delete left;
    delete LeafObject;
}

void kdTreeNode::drawRecursively(void)
{
    // Make line
    float line[] = {
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

    unsigned int buffer; // The ID, kind of a pointer for VRAM
    glGenBuffers(1, &buffer); // Allocate memory for the triangle
    glBindBuffer(GL_ARRAY_BUFFER, buffer); // Set the buffer as the active array
    glBufferData(GL_ARRAY_BUFFER, 72 * sizeof(float), line, GL_STATIC_DRAW); // Fill the buffer with data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0); // Specify how the buffer is converted to vertices
    glEnableVertexAttribArray(0); // Enable the vertex array

    // Draw the lines
    glDrawArrays(GL_LINES, 0, 2);

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

void kdTreeNode::MergeAABB(void)
{
	AABBmax.x = std::fmaxf(left->AABBmax.x, right->AABBmax.x);
	AABBmax.y = std::fmaxf(left->AABBmax.y, right->AABBmax.y);
	AABBmax.z = std::fmaxf(left->AABBmax.z, right->AABBmax.z);

	AABBmin.x = std::fminf(left->AABBmin.x, right->AABBmin.x);
	AABBmin.y = std::fminf(left->AABBmin.y, right->AABBmin.y);
	AABBmin.z = std::fminf(left->AABBmin.z, right->AABBmin.z);
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
        std::sort(triangles.begin(), triangles.end(), [](const Triangle& t1, const Triangle& t2) {
            return t1.center.x < t2.center.x;
            });
        break;
    case Axis::y:
        std::sort(triangles.begin(), triangles.end(), [](const Triangle& t1, const Triangle& t2) {
            return t1.center.y < t2.center.y;
            });
        break;
    case Axis::z:
        std::sort(triangles.begin(), triangles.end(), [](const Triangle& t1, const Triangle& t2) {
            return t1.center.z < t2.center.z;
            });
        break;
    }
}