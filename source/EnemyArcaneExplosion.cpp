#include "EnemyArcaneExplosion.h"

EnemyArcaneExplosion::EnemyArcaneExplosion() : EnemySpell(){
    super();
    life = 1.75f;
}

EnemyArcaneExplosion::~EnemyArcaneExplosion(){}

void EnemyArcaneExplosion::update(float timestep, QuadTree tree)
{

    life -= timestep;
    if (life > 0)
    {
        setScale(getScale() + (timestep));
        Eigen::Vector3f rotation = getRot() + Eigen::Vector3f(0.0, 0.1, 0.0);
        setRot(rotation.x(), rotation.y(), rotation.z());
    }
    super::update(timestep, tree);
}

void EnemyArcaneExplosion::collide(GameObject* other)
{
    other->takeDamage(.005);
}
