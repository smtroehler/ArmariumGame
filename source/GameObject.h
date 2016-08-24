#pragma  once
#ifndef __GAMEOBJECT__
#define __GAMEOBJECT__


#define EIGEN_DONT_VECTORIZE
#define EIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT
#define EIGEN_DONT_ALIGN

#include "ShapeObj.h"
#define EIGEN_DONT_VECTORIZE
#define EIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT
#define EIGEN_DONT_ALIGN
#include "../dependencies/Eigen/Geometry"
#include <queue>
#include "CubeMap.h"

class QuadTree;

class GameObject{
public:
    GameObject();
    void init(ShapeObj *sh, Eigen::Vector3f p, float x, float y, float z, float s, bool c);
    

    void setShape(ShapeObj *s);
    void setVel(Eigen::Vector3f v);
    void setWorldSize(float size);
    void setRot(float x, float y, float z);
    void setScale(float s);
    void setLife(float l);

    void setLocation(Eigen::Vector2f loc);
    void setLocation(Eigen::Vector3f loc);
    void updateLocation(Eigen::Vector2f vec);
    void updateLocation(Eigen::Vector3f vec);

    virtual bool shouldRemoveFromWorld();

    float takeDamage(float dmg);

    Eigen::Vector2f getPosition(){ return pos; }
    Eigen::Vector3f getCenter(){ return center; }
    Eigen::Vector3f getVel() { return vel; }
    ShapeObj *getShape(){ return shape; }
    float getRadius(){ return radius; }
    float getBaseRadius(){ return baseRadius; }
    float getRadius3D(){ return radius3D; }
    float getxHalf(){ return xHalf; }
    float getzHalf(){ return zHalf; }
    float getLife() { return life; }
    float getScale() { return scale; }
    bool getCollidable(){ return collidable; }
    Eigen::Vector3f getRot() { return Eigen::Vector3f(rotx, roty, rotz); }
    bool getColType(){ return useRadius; }


    void drawBounding(MatrixStack *mv, ShapeObj *drawn);
    void drawEnv(MatrixStack* mv, CubeMap env);
    bool inViewFrustum();
    bool checkCollision(GameObject* o);
    Eigen::Vector2f reposition(GameObject* o, Eigen::Vector2f);

    void ghettoCalcPath();
    

    void bindProg(Program *toBind) {shape->bindProg(toBind);}

    void setUseRadius(bool use) { useRadius = use; }
    void setRadScaler(float scaler) { radScaler = scaler, radius = baseRadius * scaler; }

    void setMat(int mat);
    virtual void draw(MatrixStack *mv);
    void shadowDraw(MatrixStack *mv);
    virtual void update(float timestep);

    void initPath(Eigen::Vector2f dest, QuadTree *tree);
    std::queue<Eigen::Vector2f> CalcPath(Eigen::Vector2f start, Eigen::Vector2f goal, QuadTree *tree);


    std::queue<Eigen::Vector2f> generateSplineInclusive(std::vector<Eigen::Vector2f> points, int numPoints);
    std::queue<Eigen::Vector2f> generateSpline(std::vector<Eigen::Vector2f> points, int numPoints);

    int animFrame = 0;

protected:
    ShapeObj *shape;           //could be a vector if object has multiple meshes
    float rotx;
    float roty;
    float rotz;
    float worldSize;
    float enemySpeed;

    float life;

    bool collidable;
    bool useRadius;

    Eigen::Vector3f vel; 

    std::queue<Eigen::Vector2f> pathList;
    bool usePath;
    float maxSpeed;

    bool removeFromWorld;
private:
    Eigen::Vector2f pos;
    Eigen::Vector3f center;
    float scale, radius, baseRadius, radScaler, radius3D;
    float xHalf, zHalf;
};

#endif
