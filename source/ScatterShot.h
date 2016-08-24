#pragma  once
#ifndef __SCATTERSHOT__
#define __SCATTERSHOT__

#include "Spell.h"
#include "Light.h"
#include "MatrixStack.h"
class ScatterShot: public Spell
{
public:
	
	ScatterShot();
	~ScatterShot();
    virtual void update(float timestep, QuadTree tree);
	void collide(GameObject* other);
	void setLight(Light *l);
	void draw(MatrixStack *MV);
private:
	typedef Spell super;
	Light *light;
};

#endif
