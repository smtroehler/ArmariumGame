#ifndef _BOOKMONSTER_H_
#define _BOOKMONSTER_H_


#include "Enemy.h"
#include "Program.h"

class Bookmonster : public Enemy
{
    public:
    Bookmonster();
    void init(Eigen::Vector3f loc, float x, float y, float z, float s, bool c);
    void init(ShapeObj *monster, Eigen::Vector3f loc, float x, float y, float z, float s, bool c);
    void bindProg(Program* prog);
    void update(float timestep, QuadTree tree);

    virtual bool shouldRemoveFromWorld();

    int frame = 0;
    //void draw(MatrixStack *mv);

    bool explode = false;
    bool die = false;
    virtual void draw(MatrixStack *MV);
    int useAnim;
    float explodeTime = 0;
    float dieTime = 0;

};
#endif