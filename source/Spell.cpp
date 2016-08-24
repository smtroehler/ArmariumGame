#include "Spell.h"
#include "PlayerObject.h"
#include "Enemy.h"
#include "EnemySpell.h"
Spell::Spell() : GameObject(){
    super();
    life = 5.0f;
    sceneCollidable = false;
}

Spell::~Spell(){}

void Spell::init(Program prog, ShapeObj *sh, Eigen::Vector3f p, float x, float y, float z, float s, bool c)
{
    prog = prog;
    super::init(sh, p, x, y, z, s, true);
    sh->setMat(MAT_PURPLE);
    sceneCollidable = c;
}

void Spell::update(float timestep, QuadTree tree){

    vector<GameObject*> collisions = tree.getCollisions(this);

    for (int i = 0; i < collisions.size(); i++){
        if (Enemy *enemy = dynamic_cast<Enemy *>(collisions[i])) //see if you are hitting an Enemy
        {
            if (checkCollision(collisions[i]) == true){
                collide(enemy);
            }
        }
        else if (dynamic_cast<PlayerObject *>(collisions[i]) == NULL && 
            dynamic_cast<Spell *>(collisions[i]) == NULL && 
            dynamic_cast<EnemySpell *>(collisions[i]) == NULL && 
            checkCollision(collisions[i]) == true){
            
           
            //hit scenery.  destroy spell only.
            if(sceneCollidable){
                takeDamage(life);
            }
        }
    }
}

void Spell::setReference(std::vector<GameObject *>& movingObj)
{
    movingObjects = movingObj;
}
