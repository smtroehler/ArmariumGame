#include "EnemySpell.h"
#include "PlayerObject.h"
#include "Spell.h"
#include <stdio.h>
EnemySpell::EnemySpell() : GameObject(){
    super();
    life = 5.0f;
    sceneCollidable = false;
}

EnemySpell::~EnemySpell(){}

void EnemySpell::init(Program prog, ShapeObj *sh, Eigen::Vector3f p, float x, float y, float z, float s, bool c)
{
    prog = prog;
    super::init(sh, p, x, y, z, s, true);
    sh->setMat(MAT_PURPLE);
    sceneCollidable = c;
}

void EnemySpell::update(float timestep, QuadTree tree){

    vector<GameObject*> collisions = tree.getCollisions(this);
    for (int i = 0; i < collisions.size(); i++){

        if (PlayerObject *player = dynamic_cast<PlayerObject *>(collisions[i])) //see if you are hitting an Enemy
        {
       
            if (checkCollision(collisions[i]) == true){
                collide(player);
            }
        }
        if (collisions[i]->getShape()->fileName == "Assets/bookstand.obj") //see if you are hitting an Enemy
        {
            if (checkCollision(collisions[i]) == true){
                playerRef->damageBook(.004);
            }
        }
        else if (dynamic_cast<PlayerObject *>(collisions[i]) == NULL && dynamic_cast<EnemySpell *>(collisions[i])  == NULL  &&
                 dynamic_cast<Spell *>(collisions[i])  == NULL && checkCollision(collisions[i]) == true){
            //hit scenery.  destroy spell only.
            if(sceneCollidable){
                takeDamage(life);
            }
        }
    }
}

void EnemySpell::setReference(std::vector<GameObject *>& movingObj, PlayerObject *player)
{
    movingObjects = movingObj;
    playerRef = player;
}
