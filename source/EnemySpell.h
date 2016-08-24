#pragma  once
#ifndef __ENEMYSPELL__
#define __ENEMYSPELL__

#include "../dependencies/Eigen/Dense"
#include "GLSL.h"
#include "Program.h"
#include "ShapeObj.h"
#include "MatrixStack.h"
#include <stdlib.h>
#include "GameObject.h"
#include "Enemy.h"
#include "Light.h"
#include "PlayerObject.h"
class EnemySpell: public GameObject
{
public:
	
	EnemySpell();
	~EnemySpell();
	void init(Program, ShapeObj *, Eigen::Vector3f, float, float, float, float, bool);
    virtual void update(float timestep, QuadTree tree);
    void setReference(std::vector<GameObject *>& movingObj, PlayerObject *player);
    void collisionResponse();
    Light *light;
protected:
	Program prog;
	bool sceneCollidable;
	typedef GameObject super;
    std::vector<GameObject *> movingObjects;
    virtual void collide(GameObject *){};
    PlayerObject *playerRef;

};

#endif
