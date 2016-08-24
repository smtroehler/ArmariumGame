#pragma  once
#ifndef __PARTICLE_H__
#define __PARTICLE_H__
#define EIGEN_DONT_VECTORIZE
#define EIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT
#define EIGEN_DONT_ALIGN

#include "GameObject.h"
#include "../dependencies/Eigen/Dense"

class Particle : public GameObject{
public:
	char r,g,b,a;
	float life;
	float distToCamera;
	//void init();
	//void update(float dt);
	//void refresh();
};

#endif