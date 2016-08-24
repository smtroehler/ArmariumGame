#include "GameObject.h"
#include <iostream>
#include "Camera.h"
#include <vector>
#include <list>
#include <cassert>
#include "QuadTree.h"

extern Camera camera;

using namespace std;

typedef struct Node{
    Eigen::Vector2f pos;
    float g;
    float f;
}Node;


//Initializes location to 0 but DOES NOT set up shape.
GameObject::GameObject(){
    pos = Eigen::Vector2f(0, 0);
    center = Eigen::Vector3f(0, 0, 0);
    vel = Eigen::Vector3f(0, 0, 0);
    rotx = 0;
    roty = 0;
    rotz = 0;
    scale = 1.0;
    worldSize = 0.0;
    collidable = true;
    radius = 1.0;
    radius3D = 1.0;
    life = 1.0f;
    maxSpeed = 1.0;
    usePath = false;
    useRadius = false;
    enemySpeed = .8;
    radScaler = 1.0;
}

//Same as all below but in one line of code.
void GameObject::init(ShapeObj *sh, Eigen::Vector3f loc, float x, float y, float z, float s, bool c){
    shape = sh;
    pos = Eigen::Vector2f(loc.x(), loc.z());
    center = loc;
    rotx = x;
    roty = y;
    rotz = z;
    scale = s;
    usePath = false;
    baseRadius = sh->getRadius() * scale;
    radius = baseRadius * radScaler;
    radius3D = sh->getRadius3D() * scale;
    if ((roty * 180 / M_PI < 90.01 && roty * 180 / M_PI > 89.99) || (roty * 180 / M_PI < 270.01 && roty * 180 / M_PI > 269.99)) { //if rotated swap x and z
        xHalf = sh->getzHalf() * scale;
        zHalf = sh->getxHalf() * scale;
    }
    else {
        xHalf = sh->getxHalf() * scale;
        zHalf = sh->getzHalf() * scale;
    }
    collidable = c;
    maxSpeed = 3.0;
    useRadius = false;
}




std::queue<Eigen::Vector2f> makePath(std::vector<Node> cameFrom, Eigen::Vector2f current){
    queue<Eigen::Vector2f> ret;

    //Create path and combign points that are in the same direction.

    Eigen::Vector2f lastHeading, curHeading;
    bool first = true;
    while (cameFrom.size() > 1){
        Eigen::Vector2f temp = cameFrom.front().pos;
        cameFrom.erase(cameFrom.begin());
        curHeading = temp - cameFrom.front().pos;
        if (first || curHeading != lastHeading){
            first = false;
            ret.push(temp);
        }
        lastHeading = curHeading;
    }
    ret.push(cameFrom.front().pos);
    ret.push(current);
    return ret;
}

bool GameObject::shouldRemoveFromWorld() {
    if(life <= 0.0)
        return true;
    else
        return false;
}


std::queue<Eigen::Vector2f> GameObject::CalcPath(Eigen::Vector2f start, Eigen::Vector2f goal, QuadTree *tree){

    float moveStep = 0.25f;
    float xbound = 28;
    float zbound = 15;

    std::vector<Node> open, closed, cameFrom;

    //The 8 possible directions we can move in.  Could be expanded but tree will get much larger
    std::vector<Eigen::Vector2f> dirs;

    dirs.push_back(Eigen::Vector2f(0, -1));
    dirs.push_back(Eigen::Vector2f(0, 1));
    dirs.push_back(Eigen::Vector2f(-1, 0));
    dirs.push_back(Eigen::Vector2f(1, 0));

    dirs.push_back(Eigen::Vector2f(1, -1));
    dirs.push_back(Eigen::Vector2f(1, 1));
    dirs.push_back(Eigen::Vector2f(-1, -1));
    dirs.push_back(Eigen::Vector2f(-1, 1));

    Node current, temp;
    current.pos = start;
    current.g = 0.0;
    current.f = (goal - start).norm();
    open.push_back(current);
    printf("start f: %f\n", current.f);

    while (open.size() != 0 && open.size() < 15000){
        printf("open: %d\n", open.size());
        //current is open with lowest f value
        float lowest = open.front().f;
        int low = 0;
        for (unsigned int i = 1; i < open.size(); i++){
            if (open[i].f < lowest)
                low = i;
        }
        current = open[low];
        //Check if node is goal, return the path to get there.
        if ((goal - current.pos).norm() < 0.3){
            printf("found goal\n");
            //return makePath(open, goal);
            return makePath(cameFrom, goal);
        }
        //Otherwise, move to closed
        current = open[low];
        open.erase(open.begin() + low);
        closed.push_back(current);
        //calculate g score for every neighbour adding new neighbours to open with current node as part of cameFrom
        //expand and stuff
        for (int i = 0; i < dirs.size(); i++){
            temp.pos = current.pos + dirs[i] * moveStep;

            //check if already in closed set
            bool inClosed = false;
            for (int j = 0; j < closed.size(); j++){
                if (closed[j].pos == temp.pos)
                    inClosed = true;
            }

            if (inClosed == false){
                //check if within world
                if (temp.pos.x() > -xbound && temp.pos.x() < xbound &&
                    temp.pos.y() > -zbound && temp.pos.y() < zbound){

                    printf("checking x:%f z:%f\n", temp.pos.x(), temp.pos.y());
                    //check if colliding
                    vector<GameObject*> collisions = tree->getCollisions(this);
                    bool collided = false;

                    //printf("numcols: %d\n", collisions.size());
                    for (int j = 0; j < collisions.size(); j++){
                        if (checkCollision(collisions[j]) == true)
                            collided == true;
                    }

                    if (collided == false){
                        //not colliding! calculate g and f
                        temp.g = current.g + (dirs[i] * moveStep).norm();

                        printf("f: %f\n", temp.f);
                        //if lower than open and closed, add to open
                        bool add = true;
                        bool notopen = true;
                        for (int j = 0; j < open.size(); j++){
                            if (temp.g > open[j].g){
                                add = false;
                                break;
                            }
                            //add anyway if not in open.
                            if (temp.pos == open[j].pos){
                                notopen = false;
                                break;
                            }
                        }
                        for (int j = 0; j < closed.size(); j++){
                            if (temp.g > closed[j].g){
                                add = false;
                                break;
                            }
                        }

                        if (add || notopen){
                            temp.f = temp.g + (goal - temp.pos).norm();
                            open.push_back(temp);
                            cameFrom.push_back(current);
                        }
                    }
                    else
                        printf("possible collision\n");
                }
            }
        }
    }

    printf("No path found...\n");
    std::queue<Eigen::Vector2f> empty;
    return empty;
}

//hard code some paths.  hacked together for 75% and will be removed when i get A* working
void GameObject::ghettoCalcPath(){
    std::vector<Eigen::Vector2f> path;
    usePath = true;
    //up right
    if (pos.x() == 20 && pos.y() == -14){
        path.push_back(Eigen::Vector2f(20, -11));
        path.push_back(Eigen::Vector2f(1, -11));
        path.push_back(Eigen::Vector2f(1, -1));
    }

    //up left
    if (pos.x() == -20 && pos.y() == -14){
        path.push_back(Eigen::Vector2f(-20, -11));
        path.push_back(Eigen::Vector2f(-24, -11));
        path.push_back(Eigen::Vector2f(-24, -4));
        path.push_back(Eigen::Vector2f(-1, -1));
    }

    //far right
    if (pos.x() == 25 && pos.y() == 4){
        path.push_back(Eigen::Vector2f(25, 13));
        path.push_back(Eigen::Vector2f(20, 13));
        path.push_back(Eigen::Vector2f(15, 13));
        path.push_back(Eigen::Vector2f(15, 7.5));
        path.push_back(Eigen::Vector2f(15, 5));
        path.push_back(Eigen::Vector2f(7, 2.5));
        path.push_back(Eigen::Vector2f(1, 0));
    }

    //far left
    if (pos.x() == -25 && pos.y() == 0){
        path.push_back(Eigen::Vector2f(-25, 10));
        path.push_back(Eigen::Vector2f(-15, 10));
        path.push_back(Eigen::Vector2f(-15, 3));
        path.push_back(Eigen::Vector2f(-1, 1));
    }

    pathList = generateSplineInclusive(path, 20);
}


//calculates path from current pos to dest.  Do it in ground plane.
void GameObject::initPath(Eigen::Vector2f dest, QuadTree *tree){
    usePath = true;
    //pathList = CalcPath(pos, dest, tree);
    //pathList.pop();
    vector<Eigen::Vector2f> vec = { Eigen::Vector2f(0, 0), Eigen::Vector2f(0, 0), Eigen::Vector2f(2.5, -2.5), Eigen::Vector2f(5, 0), Eigen::Vector2f(7.5, 2.5), Eigen::Vector2f(10, 0), Eigen::Vector2f(15, 1), Eigen::Vector2f(15, 1) };
    pathList = generateSpline(vec, 40);
}

void GameObject::setShape(ShapeObj *s){ //Do not use other than quad tree!
    shape = s;
}

void GameObject::setLocation(Eigen::Vector2f loc){
    pos = loc;
    center = Eigen::Vector3f(loc.x(), center.y(), loc.y());
}

void GameObject::setLocation(Eigen::Vector3f loc){
    pos = Eigen::Vector2f(loc.x(), loc.z());
    center = loc;
}

void GameObject::updateLocation(Eigen::Vector2f vec){
    pos += vec;
    center += Eigen::Vector3f(vec.x(), 0.0, vec.y());
}

void GameObject::updateLocation(Eigen::Vector3f vec){
    pos += Eigen::Vector2f(vec.x(), vec.z());
    center += vec;
}

void GameObject::setRot(float x, float y, float z){
    rotx = x;
    roty = y;
    rotz = z;
}

void GameObject::setScale(float s){
    center = Eigen::Vector3f(center.x(), center.y() - shape->getCenter().y()*scale + shape->getCenter().y()*s, center.z()); // Accounts for previous scaling
    scale = s;
    baseRadius = shape->getRadius() * scale;
    radius = baseRadius * radScaler;
    radius3D = shape->getRadius3D() * scale;
    if ((roty * 180 / M_PI < 90.01 && roty * 180 / M_PI > 89.99) || (roty * 180 / M_PI < 270.01 && roty * 180 / M_PI > 269.99)) { //if rotated swap x and z
        xHalf = getShape()->getzHalf() * scale;
        zHalf = getShape()->getxHalf() * scale;
    }
    else {
        xHalf = getShape()->getxHalf() * scale;
        zHalf = getShape()->getzHalf() * scale;
    }
}

void GameObject::setVel(Eigen::Vector3f v){
    vel = v;
}

void GameObject::setLife(float l){
    life = l;
}

void GameObject::update(float timestep) {
    updateLocation(Eigen::Vector3f(vel.x() * timestep, vel.y() * timestep, vel.z() * timestep));

    //point in direction of velocity
    if (vel.norm() != 0){
        Eigen::Vector3f start = Eigen::Vector3f(0, 0, -1);
        roty = acos(vel.dot(start) / vel.norm() * start.norm());
        if (vel.x() > 0)
            roty = (2 * M_PI) - roty;
        //bunny model is dumb
        roty += M_PI;
    }

    //Set velocity using path
    if (usePath){
        if (!pathList.empty()){

            //float move = 0;
            //if (timestep > 0) {
            //    move = maxSpeed * timestep;
            //    float distToPoint = (pathList.front() - pos).norm();
            //    //Case where we get to point.
            //    do{
            //        printf("dist: %f, move: %f timestep %f x: %f y: %f\n", distToPoint, move, timestep, pos.x(),pos.y());
            //        if (distToPoint < move){
            //            move -= distToPoint;
            //            setLocation(pathList.front());
            //            pathList.pop();
            //        }
            //    } while (!pathList.empty() && move > (distToPoint = (pathList.front() - pos).norm()));
            //}

            float move = maxSpeed;

            //Case where we get to point.

            float distToPoint = (pathList.front() - pos).norm();
            /*
            if (distToPoint < maxSpeed){
            move = maxSpeed - distToPoint;
            setLocation(pathList.front());
            pathList.pop();
            }*/

            //works for point to point but not spline
            if (distToPoint < 0.2){
                pathList.pop();
            }

            //set
            if (!pathList.empty()){
                Eigen::Vector2f temp = (pathList.front() - pos).normalized() * move;
                vel = Eigen::Vector3f(temp.x(), 0, temp.y());
            }
        }
        else{
            vel = Eigen::Vector3f(0, 0, 0);
        }
    }
    vel *= enemySpeed;

}

//Takes the GameObject's translations and applies them to a matrix.  Then sends matrix to ShapeObj for drawing.
void GameObject::draw(MatrixStack *mv){

    assert(glGetError() == GL_NO_ERROR);
    MatrixStack model;

    mv->pushMatrix();
    mv->loadIdentity();
    mv->translate(center);
    mv->rotate(rotx, Eigen::Vector3f(1, 0, 0));
    mv->rotate(roty, Eigen::Vector3f(0, 1, 0));
    mv->rotate(rotz, Eigen::Vector3f(0, 0, 1));
    mv->scale(scale);
    if (inViewFrustum()) {
        shape->draw(mv);
    }
    //mv->popMatrix();
    assert(glGetError() == GL_NO_ERROR);
}

void GameObject::drawEnv(MatrixStack* mv, CubeMap env){
    assert(glGetError() == GL_NO_ERROR);
    mv->pushMatrix();
    mv->translate(center);
    mv->rotate(rotx, Eigen::Vector3f(1, 0, 0));
    mv->rotate(roty, Eigen::Vector3f(0, 1, 0));
    mv->rotate(rotz, Eigen::Vector3f(0, 0, 1));
    mv->scale(scale);
    if (inViewFrustum()) {
        shape->drawEnv(mv, env);
    }
    mv->popMatrix();
    assert(glGetError() == GL_NO_ERROR);
}

void GameObject::drawBounding(MatrixStack *mv, ShapeObj *drawn){
    mv->pushMatrix();
    mv->translate(Eigen::Vector3f(center.x(), center.y() + baseRadius, center.z()));
    mv->rotate(rotx, Eigen::Vector3f(1, 0, 0));
    mv->rotate(roty, Eigen::Vector3f(0, 1, 0));
    mv->rotate(rotz, Eigen::Vector3f(0, 0, 1));
    mv->scale(radius); // Radius of circle for collisions.  Change to radius3D for view frustum culling sphere.

    drawn->draw(mv);

    mv->popMatrix();
}

void GameObject::shadowDraw(MatrixStack *mv){
    mv->pushMatrix();
    mv->translate(center);
    mv->rotate(rotx, Eigen::Vector3f(1, 0, 0));
    mv->rotate(roty, Eigen::Vector3f(0, 1, 0));
    mv->rotate(rotz, Eigen::Vector3f(0, 0, 1));
    mv->scale(scale);
    shape->shadowDraw(mv);
    mv->popMatrix();
}

float GameObject::takeDamage(float dmg)
{
    life -= dmg;
    return life;
}

bool GameObject::inViewFrustum() {
    Eigen::Vector4f posW;
    posW.x() = center.x();
    posW.y() = center.y();
    posW.z() = center.z();
    posW.w() = 1;

    if (posW.dot(camera.getLeftPlane()) <= -radius3D) {
        //outside left
        return false;
    }
    if (posW.dot(camera.getRightPlane()) <= -radius3D) {
        //outside right
        return false;
    }
    if (posW.dot(camera.getBottomPlane()) <= -radius3D) {
        //outside bot
        return false;
    }
    if (posW.dot(camera.getTopPlane()) <= -radius3D) {
        //outside top
        return false;
    }
    if (posW.dot(camera.getNearPlane()) <= -radius3D) {
        //outside near
        return false;
    }
    if (posW.dot(camera.getFarPlane()) <= -radius3D) {
        //outside far
        return false;
    }

    return true;
}


bool GameObject::checkCollision(GameObject* o) {
    if (!o->getCollidable() || !collidable) {
        return false;
    }
    bool oUseRadius = o->getColType();
    if (useRadius && oUseRadius){ // circle, circle CORRECT
        if ((radius + o->getRadius()) > (getPosition() - o->getPosition()).norm()) {
            return true;
        }
    }
    else if (!useRadius && !oUseRadius){ // rect, rect CORRECT
        if (abs(pos.x() - o->getPosition().x()) < xHalf + o->getxHalf() && abs(pos.y() - o->getPosition().y()) < zHalf + o->getzHalf()){
            return true;
        }
    }
    else if (useRadius && !oUseRadius){ // circle this, rect o CORRECT
        Eigen::Vector2f circleDistance;
        circleDistance.x() = abs(pos.x() - o->getPosition().x());
        circleDistance.y() = abs(pos.y() - o->getPosition().y());

        if (circleDistance.x() > o->getxHalf() + radius) { return false; }
        if (circleDistance.y() > o->getzHalf() + radius) { return false; }

        if (circleDistance.x() <= o->getxHalf()) { return true; }
        if (circleDistance.y() <= o->getzHalf()) { return true; }

        float cornerDistance_sqd = circleDistance.x() - o->getxHalf() + circleDistance.y() - o->getzHalf();

        return (cornerDistance_sqd <= radius);
    }
    else{ // rect, circle UNTESTED
        Eigen::Vector2f circleDistance;
        circleDistance.x() = abs(o->getPosition().x() - pos.x());
        circleDistance.y() = abs(o->getPosition().y() - pos.y());

        if (circleDistance.x() > xHalf + o->getRadius()) { return false; }
        if (circleDistance.y() > zHalf + o->getRadius()) { return false; }

        if (circleDistance.x() <= xHalf) { return true; }
        if (circleDistance.y() <= zHalf) { return true; }

        float cornerDistance_sqd = circleDistance.x() - xHalf + circleDistance.y() - zHalf;

        return (cornerDistance_sqd <= o->getRadius());
    }

    return false;
}


Eigen::Vector2f GameObject::reposition(GameObject* o, Eigen::Vector2f old) {
    bool oUseRadius = o->getColType();
    Eigen::Vector2f ret;
    if (useRadius && oUseRadius){ // circle, circle 
        ret = o->getPosition() + (pos - o->getPosition()).normalized() * (getRadius() + o->getRadius());
    }
    else if (!useRadius && !oUseRadius){ // rect, rect 
        cout << "ERROR rect rect" << endl;
    }
    else if (useRadius && !oUseRadius){ // circle, rect 
        Eigen::Vector2f a, b;
        
            if (pos.x() < o->getPosition().x()) {
                a.x() = o->getPosition().x() - (radius + o->getxHalf());
            }
            else {
                a.x() = o->getPosition().x() + (radius + o->getxHalf());
            }
            a.y() = pos.y();
            if (pos.y() < o->getPosition().y()) {
                b.y() = o->getPosition().x() - (radius + o->getzHalf());
            }
            else {
                b.y() = o->getPosition().x() + (radius + o->getzHalf());
            }
            b.x() = pos.x();
            if ((a - pos).norm() < (b - pos).norm()) {
                ret = a;
            }
            else {
                ret = b;
            }
            ret = old;
        //cout << "circle rect" << endl;
    }
    else{ // rect, circle 
        cout << "ERROR rect circle" << endl;
    }
    return ret;
}

//https://github.com/SneakyBrian/Catmull-Rom-Sample/blob/master/Catmull%E2%80%93Rom-Sample/CatmullRomSpline.cs

Eigen::Vector2f pointOnSpline(Eigen::Vector2f p0, Eigen::Vector2f p1, Eigen::Vector2f p2, Eigen::Vector2f p3, float t) {
    Eigen::Vector2f ret;

    float t2 = t*t;
    float t3 = t*t*t;

    ret.x() = 0.5f * ((2.0f * p1.x()) +
        (-p0.x() + p2.x()) * t +
        (2.0f * p0.x() - 5.0f * p1.x() + 4 * p2.x() - p3.x()) * t2 +
        (-p0.x() + 3.0f * p1.x() - 3.0f * p2.x() + p3.x()) * t3);

    ret.y() = 0.5f * ((2.0f * p1.y()) +
        (-p0.y() + p2.y()) * t +
        (2.0f * p0.y() - 5.0f * p1.y() + 4 * p2.y() - p3.y()) * t2 +
        (-p0.y() + 3.0f * p1.y() - 3.0f * p2.y() + p3.y()) * t3);
    return ret;
}


queue<Eigen::Vector2f> GameObject::generateSplineInclusive(vector<Eigen::Vector2f> points, int numPoints) {
    vector<Eigen::Vector2f> pointsInclusive = points;
    pointsInclusive.push_back(pointsInclusive.back());
    pointsInclusive.insert(pointsInclusive.begin(), pointsInclusive.front());
    return generateSpline(pointsInclusive, numPoints);
}

queue<Eigen::Vector2f> GameObject::generateSpline(vector<Eigen::Vector2f> points, int numPoints) {
    assert(points.size() > 3);

    queue<Eigen::Vector2f> splinePoints;

    for (int i = 0; i < points.size() - 3; i++)
    {
        for (int j = 0; j < numPoints; j++)
        {
            splinePoints.push(pointOnSpline(points[i], points[i + 1], points[i + 2], points[i + 3], (1.0f / numPoints) * j));
        }
    }

    splinePoints.push(points[points.size() - 2]);

    return splinePoints;
}


