#include "MainMenu.h"
#include "DefferedShader.h"
#include <iostream>
#include <cmath>
MainMenu::MainMenu() {

}

extern int width;
extern int height;

void MainMenu::loadScene() {
    centerPiece.load("Assets/Pylon.obj");
    centerPiece.setTexture("Assets/Textures/pylon_diffuse.jpg");
    centerPiece.init();

    centerFloor.load("Assets/FloorEnd.obj");
    centerFloor.setTexture("Assets/Textures/floor_wood.jpg");
    centerFloor.init();

    camera.position = Eigen::Vector3f(0.0,2.0,0.0);
    camera.pullBackOffset = Eigen::Vector3f( 0.0f, 0.0f,18.0f);
    camera.rotation(1) = -M_PI / 12;
    camera.rotation(0) = 0.0;

    cameraRotSpeed = 1.0f;
    camera.setAspect(width, height);

    
    SplashImage.loadFile("Assets/square.obj");
    SplashImage.setWH(width,height);
    SplashImage.setTextureName("Assets/HUD/title_screen.png");
    SplashImage.setBlendName("Assets/HUD/title_screen_overlay.png");
    SplashImage.init();


    instructions.loadFile("Assets/square.obj");
    instructions.setWH(width,height);
    instructions.setTextureName("Assets/HUD/instructions.jpg");
    instructions.setBlendName("Assets/HUD/instructions_overlay.jpg");
    instructions.init();


    SplashImage.pos = Eigen::Vector3f(width/2.0, height/2.0, 1);
    SplashImage.scale = Eigen::Vector3f(600, 600, 1);
    SplashImage.rot(0) = M_PI /2.0;

    instructions.pos = Eigen::Vector3f(width - 300, 150, 1);
    instructions.scale = Eigen::Vector3f(400, 400, 1);
    instructions.rot(0) = M_PI /2.0;



    centerP.init(&centerPiece, Eigen::Vector3f(0.0,0.0,0.0), 0, 0, 0, 1, false);
    centerF.init(&centerFloor, Eigen::Vector3f(0.0,0.0,0.0), 0, 0, 0, 5, false);

    Light *light = new Light;
    light->setPosition(Eigen::Vector3f(0.0, 4.0, 2.0));
    light->setAmbience(Eigen::Vector3f(0.1, 0.1, 0.1));
    light->setLuminance(Eigen::Vector3f(1, .6, 0.0));
    light->setAtten(0.025, 0.025, 0.025);
    lights.push_back(light);

}

float rf(float l, float h){
    float r = rand() / (float)RAND_MAX;
    return (1.0f - r) * l + r *h;
}

void MainMenu::update(float timestep) {
    static float totaltime = 0;
    totaltime += timestep * 2;
    camera.rotation(0) += timestep * .8;    
    camera.pullBackOffset(2) = 18.0 + 3.0 * sin(totaltime);

    float randtime = rf(0.0, 10.0);
  
    SplashImage.pos = Eigen::Vector3f(width/2.0, height/2.0, 1);
    lights[0]->setLuminance(Eigen::Vector3f(1.0, .5 , 0.0));

    //SplashImage.setWH(width,height);
}

extern DefferedShader def_shader;
extern Program HudShader;
extern int defferedMode;

void MainMenu::draw() {

    camera.setAspect(width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_CULL_FACE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_BACK);

    MatrixStack MV,P;

    prog->bind();
    def_shader.start();
    
    P.pushMatrix();
    camera.applyProjectionMatrix(&P);
    MV.pushMatrix();
    camera.applyViewMatrix(&MV);

    glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, P.topMatrix().data());
    glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, MV.topMatrix().data());
    
    centerPiece.bindProg(prog);
    centerFloor.bindProg(prog);

    //MV.pushMatrix();
    centerP.draw(&MV);
    centerF.draw(&MV);

  //  MV.popMatrix();

    prog->unbind();


    def_shader.bind();
    def_shader.setLightData(lights);
    def_shader.render(0, camera.getWorldPos(), lights.size());
    

    HudShader.bind();
    SplashImage.bindProg(&HudShader);
    SplashImage.draw(&P, &MV);

    instructions.bindProg(&HudShader);
    instructions.draw(&P, &MV);
    HudShader.unbind();





//    MV.popMatrix();
    P.popMatrix();
    assert(glGetError() == GL_NO_ERROR);
}