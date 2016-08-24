#pragma  once
#ifndef __ARC_EXP_SPELL__
#define __ARC_EXP_SPELL__

#include "Spell.h"

class ArcaneExplosion: public Spell
{
public:
	
	ArcaneExplosion();
	~ArcaneExplosion();
    virtual void update(float timestep, QuadTree tree);
	void collide(GameObject* other);
private:
	typedef Spell super;
};

#endif
