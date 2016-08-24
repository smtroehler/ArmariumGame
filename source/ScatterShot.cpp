#include "ScatterShot.h"

ScatterShot::ScatterShot() : Spell(){
    super();
    life = 1.75f;
}

ScatterShot::~ScatterShot(){}

void ScatterShot::setLight(Light *l)
{
    light = l;
    light->setAmbience(Eigen::Vector3f(0.15, 0.01, 0.15));
    light->setLuminance(Eigen::Vector3f(1.5, 0.3, 2.4));
    light->setAtten(0.1, 0.2, 0.5);
}

void ScatterShot::update(float timestep, QuadTree tree)
{
    life -= timestep;
    if (life > 0)
    {
        //updateLocation(Eigen::Vector3f(0.2, 0, 0));
        updateLocation(Eigen::Vector3f(vel.x() * timestep, vel.y() * timestep, vel.z() * timestep));
        light->setPosition(getCenter());
    }
    super::update(timestep, tree);
}

void ScatterShot::collide(GameObject* other)
{
    other->takeDamage(3.0f);
}

void ScatterShot::draw(MatrixStack *MV)
{
    super::draw(MV);
}