#include "QuadTree.h"
using namespace std;

QuadTree::QuadTree(int treeDepth, Bounds bounds){
    MAX_OBJECTS = 1;
    MAX_DEPTH = 8;

    depth = treeDepth;
    rect = bounds;
}

QuadTree::~QuadTree(){}

// Recursively clear the entire quadtree
void QuadTree::clear(){
    objects.clear();

    for (int i = 0; i < subNodes.size(); i++){
        objects.clear();

        subNodes[i].clear();
    }
    subNodes.clear();
}

void QuadTree::subdivide(){
    float subW = rect.getW() / 2.f;
    float subH = rect.getH() / 2.f;
    float x = rect.getX();
    float z = rect.getZ();

    Bounds subNE(x + subW, z + subH, subW, subH);
    Bounds subNW(x - subW, z + subH, subW, subH);
    Bounds subSW(x - subW, z - subH, subW, subH);
    Bounds subSE(x + subW, z - subH, subW, subH);

    QuadTree nodeNE(depth + 1, subNE);
    QuadTree nodeNW(depth + 1, subNW);
    QuadTree nodeSW(depth + 1, subSW);
    QuadTree nodeSE(depth + 1, subSE);

    subNodes.push_back(nodeNE);
    subNodes.push_back(nodeNW);
    subNodes.push_back(nodeSW);
    subNodes.push_back(nodeSE);

    for (int i = 0; i < subNodes.size(); i++){
        subNodes[i].setShape(shape);
    }
}

int QuadTree::index(GameObject* object){
    int index = -1;

    Eigen::Vector2f pos = object->getPosition();
    float r = object->getRadius();
    float posX = pos.x();
    float posZ = pos.y();

    float x = rect.getX();
    float z = rect.getZ();
    float w = rect.getW();
    float h = rect.getH();

    // Get the XZ extreme points of the object's bounding sphere
    float top = posZ + r;
    float bot = posZ - r;
    float lft = posX - r;
    float rht = posX + r;

    // Assert that the object is not in mutually exlusive quadrants
    // i.e. an object cannot be in both the top AND bot quadrants
    // or both the lft AND rht quadrants
    // assert(!top || !bot);
    // assert(!lft || !rht);

    bool topQuadrant = (bot > z);
    bool botQuadrant = (top < z);
    bool lftQuadrant = (rht < x);
    bool rhtQuadrant = (lft > x);

    if (topQuadrant){
        if (rhtQuadrant){
            index = 0;
        }
        else if (lftQuadrant){
            index = 1;
        }
    }
    else if (botQuadrant){
        if (lftQuadrant){
            index = 2;
        }
        else if (rhtQuadrant){
            index = 3;
        }
    }

    return index;
}

void QuadTree::insert(GameObject* object){
    // If the current node is already subdivided, try adding it to a child node
    if (!subNodes.empty()){
        int idx = index(object);

        // If the object fits within a child quadrant, insert it into that quadrant
        if (idx > -1){
            subNodes[idx].insert(object);
            return;
        }
    }

    // If the current node is not subdivided, or the object spans child
    // quadrants, add it to the current node
    objects.push_back(object);

    // If the newly added object increases the number of objects in the current node
    // to be greater than MAX_OBJECTS, AND MAX_DEPTH has not yet been reached, AND 
    // the current node is not yet subdivided, then subdivide the node
    if (objects.size() > MAX_OBJECTS && depth < MAX_DEPTH){
        // Don't subdivide if current node is already subdivided.
        if (subNodes.empty()){
            subdivide();
        }

        // Iterate through all objects of the current node, inserting them into
        // children nodes if possible
        int i = 0;
        while (i < objects.size()){
            int idx = index(objects[i]);

            // If the current object fits into a child quadrant, insert it into that
            // child node and remove it from the current node
            if (idx > -1){
                subNodes[idx].insert(objects[i]);
                objects.erase(objects.begin() + i);
            }
            // Otherwise, leave the object in the current node, and move on to the next
            // object
            else{
                i++;
            }
        }
    }
}

vector<GameObject*> QuadTree::getCollisions(GameObject* object){

    // Add all objects in current node to collision list
    vector<GameObject*> collisions = objects;
    vector<GameObject*> subCollisions;


    Eigen::Vector2f pos = object->getPosition();
    float r = object->getRadius();
    float posX = pos.x();
    float posZ = pos.y();

    float x = rect.getX();
    float z = rect.getZ();
    float w = rect.getW();
    float h = rect.getH();

    // Get the XZ extreme points of the object's bounding sphere
    float top = posZ + r;
    float bot = posZ - r;
    float lft = posX - r;
    float rht = posX + r;

    bool topQuadrant = (top > z);
    bool botQuadrant = (bot < z);
    bool lftQuadrant = (lft < x);
    bool rhtQuadrant = (rht > x);

    vector<int> indices;

    if(topQuadrant){
    	if(rhtQuadrant){
    		indices.push_back(0);
    	}
    	if(lftQuadrant){
    		indices.push_back(1);
    	}
    }

    if(botQuadrant){
    	if(lftQuadrant){
    		indices.push_back(2);
    	}
    	if(rhtQuadrant){
    		indices.push_back(3);
    	}
    }

    for(int i = 0; i < indices.size() && !subNodes.empty(); i++){
    	int idx = indices[i];
    	subCollisions = subNodes[idx].getCollisions(object);
    	collisions.insert(collisions.end(), subCollisions.begin(), subCollisions.end());
    }
    

    // Combine collision lists and return
    return collisions;
}

void QuadTree::drawQuadTree(MatrixStack* mv){
    mv->pushMatrix();
    mv->translate(Eigen::Vector3f(rect.getX(), 0.0, rect.getZ()));
    mv->scale(Eigen::Vector3f(rect.getW() * 2, 1.0, rect.getH() * 2));
    shape->draw(mv);
    mv->popMatrix();


    if (!subNodes.empty()){
        for (int i = 0; i < subNodes.size(); i++){
            subNodes[i].drawQuadTree(mv);
        }
    }
}

void QuadTree::test(){
    cout << "Current bounds are (" << rect.getX() - rect.getW() << ", " << rect.getZ() - rect.getH() << ")x(";
    cout << rect.getX() + rect.getW() << ", " << rect.getZ() + rect.getH() << ")\n";
}
