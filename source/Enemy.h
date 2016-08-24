#ifndef _ENEMY_H_
#define _ENEMY_H_


#define EIGEN_DONT_VECTORIZE
#define EIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT
#define EIGEN_DONT_ALIGN
#include "MatrixStack.h"
#include "GLSL.h"
#include "GameObject.h"
#include "../dependencies/Eigen/Dense"
#include "SceneObject.h"

class Enemy : public GameObject
{
public: 
   Enemy();
   virtual ~Enemy();
   void draw(MatrixStack *mv);
   void update(float timestep, QuadTree tree);
   void setReferences(GameObject *playerObject, SceneObject *sceneObject);
   void (*spellCallback)(Eigen::Vector3f);
protected:
   typedef GameObject super;
   GameObject *player;
   SceneObject *scene;

   float enemySpeed;
};

#endif