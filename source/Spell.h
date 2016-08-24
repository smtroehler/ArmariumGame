#pragma  once
#ifndef __SPELL__
#define __SPELL__

#include "../dependencies/Eigen/Dense"
#include "GLSL.h"
#include "Program.h"
#include "ShapeObj.h"
#include "MatrixStack.h"
#include <stdlib.h>
#include "GameObject.h"
#include "Enemy.h"
#include "Light.h"
class Spell: public GameObject
{
public:
	
	Spell();
	~Spell();
	void init(Program, ShapeObj *, Eigen::Vector3f, float, float, float, float, bool);
    virtual void update(float timestep, QuadTree tree);
    void setReference(std::vector<GameObject *>& movingObj);
    void collisionResponse();
    Light *light;
protected:
	Program prog;
	bool sceneCollidable;
	typedef GameObject super;
    std::vector<GameObject *> movingObjects;
    virtual void collide(GameObject *){};

};

#endif
