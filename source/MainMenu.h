#pragma once
#ifndef _MAINMENU_H_
#define _MAINMENU_H_
#define EIGEN_DONT_VECTORIZE
#define EIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT
#define EIGEN_DONT_ALIGN


#include "Program.h"
#include "MatrixStack.h"
#include "Texture.h"
#include <string>
#include "ShapeObj.h"
#include "HudObject.h"
#include "Light.h"

class MainMenu {
    public:
    MainMenu();

    void loadScene();
    void bindProg(Program *toBind) {prog = toBind;}
    void update(float timestep);
    void draw();
    GameObject centerP, centerF;

    private:

    Program *prog;

    float cameraRotSpeed;
    ShapeObj centerPiece, centerFloor;
    Camera camera;
    HudObject SplashImage;
    HudObject instructions;

    std::vector<Light*> lights;

};
#endif