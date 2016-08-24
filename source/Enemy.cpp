#define EIGEN_DONT_VECTORIZE
#define EIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT
#define EIGEN_DONT_ALIGN
#include "Enemy.h"
#include "Spell.h"
#include "PlayerObject.h"

using namespace std;

Enemy::Enemy() :
GameObject(),
enemySpeed(1.0)
{

}

Enemy::~Enemy()
{
}

void Enemy::draw(MatrixStack *mv){
    GameObject::draw(mv);
}

void Enemy::update(float timestep, QuadTree tree) {
    
    GameObject::update(timestep);


    //Enemies collide with environment and player.  Not other enemies for now...
    //tree.getCollisions is slow...
    vector<GameObject*> collisions = tree.getCollisions(this);
    //printf("Num collisions: %d\n", collisions.size());
    for (int i = 0; i < collisions.size(); i++){
        if (dynamic_cast<Enemy *>(collisions[i]) == NULL  && dynamic_cast<Spell *>(collisions[i]) == NULL  && checkCollision(collisions[i]) == true){
            if (dynamic_cast<PlayerObject *>(collisions[i])){
              //  player->takeDamage(0.25f);
                takeDamage(99999999999999999999.f);
            }
            else if (collisions[i]->getShape()->fileName == "Assets/bookstand.obj") {
                PlayerObject* playPtr = dynamic_cast<PlayerObject *>(player) ;
           //     playPtr->damageBook(0.1f);
                takeDamage(99999999999999999999.f);
            }
            else{
                //Environment
            }
        }
    }
    
}

void Enemy::setReferences(GameObject *playerObject, SceneObject *sceneObject){
    player = playerObject;
    scene = sceneObject;
}
