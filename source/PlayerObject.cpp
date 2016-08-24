
#include "PlayerObject.h"
#include "../dependencies/Eigen/Dense"
#include "Enemy.h"
#include "Spell.h"
#include "EnemySpell.h"
using namespace std;
using namespace Eigen;

PlayerObject::PlayerObject() :
GameObject()
{
    cooldowns[0] = 0.0f;
    cooldowns[1] = 0.0f;
    cooldowns[2] = 0.0f;
    active = true;
    cooldowns[0] = 0.0f;
    cooldowns[1] = 0.0f;
    horizontalBound = 15;
    verticalBound = 28;
    setRadScaler(0.71);
    bookHealth = 1.0;
}

PlayerObject::~PlayerObject()
{

}

void PlayerObject::restart(Eigen::Vector3f p) {
    setLocation(p);
    setLife(1.0);
    bookHealth = 1.0;
}

void PlayerObject::init(Eigen::Vector3f p, float x, float y, float z, float s, bool c) {
    ShapeObj *wiz = new ShapeObj();
    wiz->loadSmd("Assets/smd_models/wizard/wizard.smd");
    wiz->loadSmdAnim("Assets/smd_models/wizard/animations/wizard_walk.smd");
    wiz->AnimationToMatrix(1);
    wiz->setTexture("Assets/Textures/wizard_texture.jpg");
    wiz->init();
    bookHealth = 1.0;
    GameObject::init(wiz, p,x,y,z,s,c);
}


float mfactor = 0.05f;

void PlayerObject::draw(MatrixStack *mv){
    if (bookHealth <= 0 || life <= 0){
        active = false;
    }
    else
        active = true;
    GameObject::draw(mv);
}

void PlayerObject::damageBook(float dam) {
    bookHealth -= dam;
}

int frame = 0;
void PlayerObject::update(GLFWwindow* window, float timestep, int width, int height,
    QuadTree tree, Eigen::Vector3f cam, MatrixStack *mv, MatrixStack *p, MatrixStack *v) {

    /*if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        active = !active;*/

    if (active) {

        cooldowns[0] -= timestep;
        cooldowns[1] -= timestep;
        cooldowns[2] -= timestep;

        // Get cursor position
        double xPixel, yPixel;
        glfwGetCursorPos(window, &xPixel, &yPixel);

        // Get current glfw width and height
        int w, h;
        glfwGetWindowSize(window, &w, &h);

        // Convert cursor position to world coordinates
        float x = (2.0 * xPixel / w) - 1.0;
        float y = 1.0 - (2 * yPixel / h);

        // Unproject the vector
        Eigen::Vector4f rayClip(x, y, -1.0, 1.0);
        float delta = rayClip.norm();
        Eigen::Vector4f rayEye = p->topMatrix().inverse() * rayClip;
        rayEye.w() = 0.0;


        rayEye = (v->topMatrix().inverse() * rayEye);
        Eigen::Vector3f rayWorld(rayEye.x(), rayEye.y(), rayEye.z());
        rayWorld.normalize();

        Eigen::Vector3f normal(0.0, 1.0, 0.0);
        float t = -(cam.dot(normal) - delta) / (rayWorld.dot(normal));
        Eigen::Vector3f origin = cam + t*rayWorld;
        origin.y() = 0;

        // Test to face towards origin
        Eigen::Vector3f aimDirection = origin - Eigen::Vector3f(getPosition().x(), 0.0, getPosition().y());
        Eigen::Vector3f faceDirection(0.0, 0.0, 1.0);

        if (aimDirection != faceDirection && aimDirection != Eigen::Vector3f(0.0, 0.0, 0.0)){
            roty = acos((aimDirection.dot(faceDirection)) / (aimDirection.norm() * faceDirection.norm()));
        }
        if (aimDirection.x() < 0){
            roty *= -1;
        }

        Eigen::Vector2f pos = getPosition();
        Eigen::Vector2f old = pos;



        // mfactor is movement speed
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && (getRadius() + pos.y()) < horizontalBound)
            //  pos -= dir * timestep * 5;
            pos += Eigen::Vector2f(0.0, 1.0) * timestep * 5;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && (pos.y() - getRadius()) > -horizontalBound)
            //  pos += dir * timestep * 5;
            pos -= Eigen::Vector2f(0.0, 1.0) * timestep * 5;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && (pos.x() - getRadius()) > -verticalBound)
            //   pos -= side * timestep * 5;
            pos -= Eigen::Vector2f(1.0, 0.0) * timestep * 5;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && (pos.x() + getRadius()) < verticalBound)
            //  pos += side  * timestep * 5;
            pos += Eigen::Vector2f(1.0, 0.0) * timestep * 5;

        if (old(0) != pos(0) || old(1) != pos(1))
            shape->animate(frame++, 1);
        setLocation(pos);
        //Player only collides with scenery.

        useRadius = true;
        vector<GameObject*> collisions = tree.getCollisions(this);
        bool colliding;
        do {
            colliding = false;
            for (int i = 0; i < collisions.size(); i++){
                if (dynamic_cast<PlayerObject *>(collisions[i]) == NULL && 
                    dynamic_cast<Enemy *>(collisions[i]) == NULL && 
                    dynamic_cast<Spell *>(collisions[i]) == NULL && 
                    dynamic_cast<EnemySpell *>(collisions[i]) == NULL && 
                    checkCollision(collisions[i]) == true){
                    setLocation(reposition(collisions[i], old));
                }
            }
        } while (colliding);


        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS){
            setLocation(Eigen::Vector2f(0, -3));
        }
    }
}


