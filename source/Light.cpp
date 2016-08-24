#include "Light.h"
#include "MatrixStack.h"
#include <iostream>
//#include "../dependencies/Eigen/Dense"

Light::Light() :
	aspect(1.0f),
	fovy(45.0f/180.0f*M_PI),
	znear(2.0f),
	zfar(8.0f),
	position(5.0f, 5.0f, 3.0f),
	target(0.0f, 0.0f, 0.0f),
	up(0.0f, 1.0f, 0.0f)
{
}

Light::~Light()
{
	
}

void Light::applyProjectionMatrix(MatrixStack *P) const
{
	//P->perspective(fovy, aspect, znear, zfar);
    P->ortho(-10, 10, -10, 10, -1, 20);
}

void Light::applyViewMatrix(MatrixStack *MV) const
{
	MV->lookAt(position, target, up);
}
