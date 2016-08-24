#pragma  once
#ifndef __PLAYEROBJECT__
#define __PLAYEROBJECT__

#include "MatrixStack.h"
#include "GLSL.h"
#include "GameObject.h"
#include "QuadTree.h"

class PlayerObject : public GameObject
{
public:
    PlayerObject();
    virtual ~PlayerObject();
    void init(Eigen::Vector3f p, float x, float y, float z, float s, bool c);
    void draw(MatrixStack *mv);
    void update(GLFWwindow* window, float timestep, int width, int height,
        QuadTree tree, Eigen::Vector3f cam, MatrixStack *mv, MatrixStack *p, MatrixStack *v);
    float cooldowns[3];
    void damageBook(float dam);
    float getBookHealth() {return bookHealth; }
    void restart(Eigen::Vector3f p);
private:
    float bookHealth;
    float camDist;
    float horizontalBound, verticalBound;
    Eigen::Vector2f mousePrev;
    bool active;
};

#endif