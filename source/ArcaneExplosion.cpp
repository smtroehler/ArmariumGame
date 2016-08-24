#include "ArcaneExplosion.h"

ArcaneExplosion::ArcaneExplosion() : Spell(){
    super();
    life = 1.75f;
}

ArcaneExplosion::~ArcaneExplosion(){}

void ArcaneExplosion::update(float timestep, QuadTree tree)
{
    life -= timestep;
    if (life > 0)
    {
        setScale(getScale() + (2.0 * timestep));
        Eigen::Vector3f rotation = getRot() + Eigen::Vector3f(0.0, 0.1, 0.0);
        setRot(rotation.x(), rotation.y(), rotation.z());
    }
    super::update(timestep, tree);
}

void ArcaneExplosion::collide(GameObject* other)
{
    other->takeDamage(3.0f);
}
