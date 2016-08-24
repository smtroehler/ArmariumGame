#include "Camera.h"
#include "MatrixStack.h"
#include <iostream>



using namespace std;
using namespace Eigen;

extern GLFWwindow *window;

Camera::Camera() :
aspect(1.0f),
fovy(30.0f / 180.0f*M_PI),
znear(0.1f),
zfar(1000.0f),
position(0.0f, 0.0f, 0.0f),
rotation(0.0f, 0.0f),
rfactor(0.01f),
mfactor(0.05f),
radius(1.0f),
dist(30.0f),
rota(-.55),
rotb(0.0f),
active(false)
{
}

Camera::~Camera()
{

}

void Camera::setAspect(float w, float h)
{
    aspect = w / h;
}

void Camera::addDist(float c){
    dist += c;
    if (dist > 50.0) {
        dist = 50.0;
    }
    if (dist < 10.0){
        dist = 10.0;
    }
}

void Camera::update(GLFWwindow* window, double timeSlice)
{

    //view frustum stuff
    MatrixStack comboMatrix;
    applyProjectionMatrix(&comboMatrix);
    applyViewMatrix(&comboMatrix);
    leftPlane = comboMatrix.topMatrix().row(3) + comboMatrix.topMatrix().row(0);
    rightPlane = comboMatrix.topMatrix().row(3) - comboMatrix.topMatrix().row(0);
    bottomPlane = comboMatrix.topMatrix().row(3) + comboMatrix.topMatrix().row(1);
    topPlane = comboMatrix.topMatrix().row(3) - comboMatrix.topMatrix().row(1);
    nearPlane = comboMatrix.topMatrix().row(3) + comboMatrix.topMatrix().row(2);
    farPlane = comboMatrix.topMatrix().row(3) - comboMatrix.topMatrix().row(2);

}

void Camera::applyProjectionMatrix(MatrixStack *P) const
{
    P->perspective(fovy, aspect, znear, zfar);
}

void Camera::applyViewMatrix(MatrixStack *MV) const
{
    MatrixStack transforms;
    // Create the translation and rotation matrices
    transforms.translate(position);
    transforms.rotate(rotation(0), Vector3f(0.0f, 1.0f, 0.0f));
    transforms.rotate(rotation(1), Vector3f(1.0f, 0.0f, 0.0f));
    transforms.translate(pullBackOffset);

    // The view matrix is the inverse
    Matrix4f V = transforms.topMatrix().inverse();
    // Add to the matrix stack
    MV->multMatrix(V);
}

void Camera::followObject(GameObject *toFollow) {

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && rota < 0)
        rota += .05;
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS && rota > -1)
        rota -= .05;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        rotb += .05;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        rotb -= .05;
    position = Vector3f(toFollow->getCenter());
    pullBackOffset = Eigen::Vector3f(0.0, 0.0, dist); //toFollow->getScale()
    rotation(1) = toFollow->getRot()(0) - 3.14f / 16 + rota;
    rotation(0) = rotb;
}

Eigen::Vector3f Camera::getViewVector()
{
    return Vector3f(cos(rotation(1)) * sin(rotation(0)), 0, cos(rotation(1)) * cos(rotation(0)));
}

Eigen::Vector3f Camera::getWorldPos()
{
    MatrixStack transforms;
    // Create the translation and rotation matrices
    transforms.translate(position);
    transforms.rotate(rotation(0), Vector3f(0.0f, 1.0f, 0.0f));
    transforms.rotate(rotation(1), Vector3f(1.0f, 0.0f, 0.0f));
    transforms.translate(pullBackOffset);
    Eigen::Vector4f out;
    out = transforms.topMatrix() * Eigen::Vector4f(0.0, 0.0, 0.0, 1.0);

    Eigen::Vector3f o;
    return out.segment(0, 3);
}