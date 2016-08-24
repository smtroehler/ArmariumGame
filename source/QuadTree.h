#ifndef _QUAD_TREE_H_
#define _QUAD_TREE_H_

#define EIGEN_DONT_VECTORIZE
#define EIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT
#define EIGEN_DONT_ALIGN

#include <iostream>
#include <vector>

#include "GameObject.h"
#include "Bounds.h"
#include "../dependencies/Eigen/Geometry"

using namespace std;

class QuadTree {

public:
    QuadTree(int treeLevel, Bounds bounds);
    virtual ~QuadTree();
    void test();

    void clear();
    void subdivide();
    int index(GameObject* object);
    void insert(GameObject* object);
    vector<GameObject*> getCollisions(GameObject* object);
    void setShape(ShapeObj* s) { shape = s; }
    void drawQuadTree(MatrixStack* mv);

private:
    ShapeObj *shape;

    int MAX_OBJECTS;
    int MAX_DEPTH;

    int depth;
    Bounds rect;

    vector<GameObject*> objects;
    vector<QuadTree> subNodes;
};

#endif
