#pragma  once
#ifndef __Camera__
#define __Camera__

#define EIGEN_DONT_VECTORIZE
#define EIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT
#define EIGEN_DONT_ALIGN
#include "../dependencies/Eigen/Dense"
#include "GLSL.h"
#include "GameObject.h"
class MatrixStack;

class Camera
{
public:

    Camera();
    virtual ~Camera();
    void setAspect(float w, float h);
    void update(GLFWwindow* window, double timeSlice);
    //void update(const bool *keys, const  Eigen::Vector2f &mouse);
    //void update(const bool *keys, const glm::vec2 &mouse);
    void applyProjectionMatrix(MatrixStack *P) const;
    void applyViewMatrix(MatrixStack *MV) const;
    void setPosition(const Eigen::Vector3f t) { position = t; }
    Eigen::Vector3f getPos() { return position; }
    float getRadius() { return radius; }
    void addDist(float c);
    Eigen::Vector3f getPullBack() { return pullBackOffset; }
    void followObject(GameObject *toFollow);
    bool active;
    Eigen::Vector3f getViewVector();
    Eigen::Vector3f getWorldPos();

    Eigen::Vector3f position;
    Eigen::Vector2f rotation;
    Eigen::Vector3f pullBackOffset;
    
    Eigen::Vector4f getNearPlane() { return nearPlane; }
    Eigen::Vector4f getFarPlane() { return farPlane; }
    Eigen::Vector4f getRightPlane() { return rightPlane; }
    Eigen::Vector4f getLeftPlane() { return leftPlane; }
    Eigen::Vector4f getTopPlane() { return topPlane; }
    Eigen::Vector4f getBottomPlane() { return bottomPlane; }

private:
    
    float dist;
    float rota, rotb;
    float aspect;
    float fovy;
    float znear;
    float zfar;
    float radius;
    Eigen::Vector2f mousePrev;
    float rfactor;
    float mfactor;
    Eigen::Vector4f nearPlane, farPlane, rightPlane, leftPlane, topPlane, bottomPlane;
};

#endif
