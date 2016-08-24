#include "FireBall.h"
#include <stdlib.h>

FireBall::FireBall() : Spell(){
    super();
    life = 2.0f;
    //radius = 0.1f;
    particleSource.init();
}

FireBall::~FireBall(){
    particleSource.~ParticleSystem();
    light->~Light();
}

void FireBall::setLight(Light *l)
{
    light = l;
    light->setAmbience(Eigen::Vector3f(0.1, 0.1, 0.1));
    light->setLuminance(Eigen::Vector3f(6.0, 1.0, 1.0));
    light->setAtten(0.1, 0.2, 0.5);
}

void FireBall::update(float timestep, QuadTree tree)
{
    life -= timestep * 0.5f;
    updateLocation(Eigen::Vector3f(vel.x() * timestep, vel.y() * timestep, vel.z() * timestep));
    particleSource.sourcePos = getCenter();
    light->setPosition(getCenter());
    if (life < 0){
        particleSource.dieing = true;
        if (!particleSource.isDead()){
            vel = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
            life = 0.0000001f;
            light->setLuminance(Eigen::Vector3f(0.0, 0.0, 0.0));
            //light->setAtten(light->getAtten().x() - 1.0 * timestep,light->getAtten().y() - 1.0 * timestep,light->getAtten().z() - 1.0 * timestep);
        }
    }
    particleSource.update(timestep);
    super::update(timestep, tree);
}

void FireBall::collide(GameObject* other)
{
    
    if(Enemy *e = dynamic_cast<Enemy *>(other)) {
        if( e->getLife() > 0.0)
            life = -1;
    }
    else {
        life = -1;
    }
    other->takeDamage(2.0f);
}

void FireBall::draw(MatrixStack *MV){
    MatrixStack P;
    P.pushMatrix();

    camera.applyProjectionMatrix(&P);
    particleSource.draw(*MV, P);
    P.popMatrix();
}

