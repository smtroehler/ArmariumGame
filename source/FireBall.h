#pragma  once
#ifndef __FIREBALL_SPELL__
#define __FIREBALL_SPELL__

//breaks if these defines removed
#define EIGEN_DONT_VECTORIZE
#define EIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT
#define EIGEN_DONT_ALIGN

#include "Spell.h"
#include "ParticleSystem.h"
#include "MatrixStack.h"
#include "Camera.h"
#include "Program.h"
#include "Light.h"
//very bad temp solution
//TODO:replace this with good solution
extern Camera camera;
extern Program prog;

class FireBall: public Spell
{
public:

        FireBall();
        ~FireBall();
        virtual void update(float timestep, QuadTree tree);
        virtual void draw(MatrixStack *MV);
        void collide(GameObject* other);
        void setLight(Light *light);
        ParticleSystem particleSource;
private:
        typedef Spell super;
        Light *light;
};

#endif

