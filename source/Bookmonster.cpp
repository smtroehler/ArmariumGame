#include "Bookmonster.h"
#include "EnemyArcaneExplosion.h"
Bookmonster::Bookmonster():
    Enemy()
{
    removeFromWorld = false;
    explode = false;
}

void Bookmonster::init(Eigen::Vector3f loc, float x, float y, float z, float s, bool c)
{
    /*ShapeObj *monster;
    monster = new ShapeObj();
    monster->loadSmd("Assets/smd_models/bookmonster/bookmonster.smd");
    monster->loadSmdAnim("Assets/smd_models/bookmonster/animations/bookmonster_running.smd");
    monster->loadSmdAnim("Assets/smd_models/bookmonster/animations/dance.smd");
    monster->setTexture("Assets/Textures/bookmonster_diffuse.jpg");
    monster->AnimationToMatrix(1);

    monster->init();
    Enemy::init(monster, loc, x,y,z,s,c);*/
}

void Bookmonster::init(ShapeObj *monster, Eigen::Vector3f loc, float x, float y, float z, float s, bool c)
{
    Enemy::init(monster, loc, x,y,z,s,c);
}


void Bookmonster::update(float timestep, QuadTree tree) 
{
    if(explode) {
        explodeTime+=timestep;
        if(explodeTime > 2.0 )  {
            explode = false;
            die = true;
            frame = 0;
        }
    }
    else if (die){
        dieTime += timestep;
        if(dieTime > 4)  {
            removeFromWorld = true;
        }
    }
    else {
        Enemy::update(timestep, tree);
      //  shape->AnimationToMatrix(1);
        shape->animate(frame++, 1);
        if(life <= 0.0) {
            explode = true;
            explodeTime = 0;
            spellCallback(getCenter());
        }

    }

}

bool Bookmonster::shouldRemoveFromWorld() {
    return removeFromWorld;
}

void Bookmonster::bindProg(Program* prog) {
    shape->bindProg(prog);
}

void Bookmonster::draw(MatrixStack *mv) {
    if(explode)
        shape->animate(frame++, 2);
    else if(die) {
        if(frame != shape->getNumFrames(3) - 1)
            frame++;

        shape->animate(frame, 3);
    }
    else
        shape->animate(frame++, 1);
    GameObject::draw(mv);
}


/*
void Bookmonster::draw(MatrixStack *mv) 
{
    prog->bind();
}*/