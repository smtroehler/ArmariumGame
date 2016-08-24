#pragma  once
#ifndef __Enemy_ARC_EXP_SPELL__
#define __Enemy_ARC_EXP_SPELL__

#include "EnemySpell.h"

class EnemyArcaneExplosion: public EnemySpell
{
public:
    
    EnemyArcaneExplosion();
    ~EnemyArcaneExplosion();
    virtual void update(float timestep, QuadTree tree);
    void collide(GameObject* other);
private:
    typedef EnemySpell super;
};

#endif
