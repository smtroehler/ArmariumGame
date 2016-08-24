
#define EIGEN_DONT_VECTORIZE
#define EIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT
#define EIGEN_DONT_ALIGN

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include <memory>
#include "../dependencies/Eigen/Dense"
#include "../dependencies/Eigen/Geometry"
#include "../dependencies/Eigen/LU"
#include "GLSL.h"
#include "Program.h"
#include "Camera.h"
#include "Light.h"

#include "MatrixStack.h"
#include "ShapeObj.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "PlayerObject.h"
#include "SceneObject.h"
#include "HudObject.h"
#include "QuadTree.h"
#include "Bounds.h"
#include "EnemySpawner.h"
#include "CubeMap.h"


#include "MainMenu.h"

//Spell includes
#include "ArcaneExplosion.h"
#include "FireBall.h"
#include "ScatterShot.h"
#include "EnemyArcaneExplosion.h"
#include "EnemySpell.h"

#include "Enemy.h"
#include "Bookmonster.h"
//particle systems
#include "ParticleSystem.h"
#include "DefferedShader.h"

#include "GameHud.h"
//GLM!
//#include "../dependencies/glm/glm.hpp"
//#include "../dependencies/glm/gtc/matrix_transform.hpp" //perspective, trans etc
//#include "../dependencies/glm/gtc/type_ptr.hpp" //value_ptr
//#include "fmod.hpp"
#include "fmod.hpp"

//#include "fmod_common.h"

//glad and glfw are included in GLSL.h
#define GROUND_SIZE 50.0f
#define ENEMY_INTERVAL 8.0f


using namespace std;

bool keyToggles[256] = { false };


bool debugPrint = false;

float gameTime = 0.0f;
float dt = 0.05f;
float spawnTime = 0.0f;
int width = 1;
int height = 1;
int score = 0;
int defferedMode = 0;
int SHADER_DEBUG = 1;
GLFWwindow* window;

Program defferedShader;
Program shadow;
Program HudShader;
Program fireballShader;

//Program torranceShader;
FMOD::System *fmod_system;
FMOD::Sound     *spell1_sound, *spell2_sound, *spell3_sound, *LossSound, *winSound, *background, *menu_song;
FMOD::Channel   *channel0 = 0, *channel1 = 0, *channel2 = 0, *channel3 = 0, *channel4 = 0;

DefferedShader def_shader;
Camera camera;
ShapeObj bunny;
ShapeObj sphere;
ShapeObj scatter;
ShapeObj quadrant;
ShapeObj wizardModel;

vector<GameObject *> movingObjects;
vector<Spell *> particleSpells;

vector<Light *> lights;
PlayerObject player;
SceneObject scene;
EnemySpawner spawner;

Bounds bounds(0, 0, 28, 15);
QuadTree tree(0, bounds);

GameObject test;
int gameState = 0;
double timeSlice = TimeManager::Instance().DeltaTime;

GLuint framebufferID;
GLuint shadowMap;
GLuint shadowmap_width = 2048;
GLuint shadowmap_height = 2048;

bool drawQuadTree = false;

bool start = false;

vector<ShapeObj> shapeStorage;

GameHud gamehud;

MainMenu menuScreen;

Light shadowLight;

//Environment mapping
Program envShader;
CubeMap env;
GameObject envMapped;

float randFloat(float l, float h){
    float r = rand() / (float)RAND_MAX;
    return (1.0f - r) * l + r *h;
}

//track pad or mouse wheel scroll.  Used to set camera distance.
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    //printf("x: %f y: %f\n", xoffset, yoffset);
    camera.addDist(-(float)yoffset);

}

void spell1() {
    if (player.cooldowns[0] <= 0.0f) {
        Light *light = new Light;
        lights.push_back(light);

        particleSpells.push_back(new FireBall());
        FireBall *ball = dynamic_cast<FireBall *>(particleSpells[particleSpells.size() - 1]);

        ball->init(defferedShader, &sphere, player.getCenter() + Eigen::Vector3f(0.0f, 0.6f, 0.0f), 0, 0, 0, 0.2, true);
        Eigen::Vector3f rotation = Eigen::Vector3f(sin((float)player.getRot()(1)), 0.0f, cos((float)player.getRot()(1)));
        ball->setVel(10.0f * rotation);
        ball->setReference(movingObjects);
        ball->particleSource.shader = fireballShader;
        assert(fmod_system->playSound(FMOD_CHANNEL_FREE, spell1_sound, false, &channel1) == FMOD_OK);
        movingObjects.push_back(ball);

        ball->setLight(light);
        player.cooldowns[0] = 1.0f;
        gamehud.used[0] = true;
    }
}

void spell2() {
    if (player.cooldowns[1] <= 0.0f) {
        movingObjects.push_back(new ArcaneExplosion());
        ArcaneExplosion *explosion = dynamic_cast<ArcaneExplosion *>(movingObjects[movingObjects.size() - 1]);
        explosion->init(defferedShader, &sphere, player.getCenter(), 0, 0, 0, 0.25, false);
        explosion->setReference(movingObjects);
        player.cooldowns[1] = 5.0f;
        assert(fmod_system->playSound(FMOD_CHANNEL_FREE, spell2_sound, false, &channel2) == FMOD_OK);
    }
}

void EnemySpellCastFunc(Eigen::Vector3f pos) {

    EnemyArcaneExplosion *eecw = new EnemyArcaneExplosion();

    movingObjects.push_back(eecw);
    EnemyArcaneExplosion *explosion = dynamic_cast<EnemyArcaneExplosion *>(movingObjects[movingObjects.size() - 1]);
    explosion->init(defferedShader, &sphere, pos, 0, 0, 0, 0.25, false);
    explosion->setReference(movingObjects, &player);
}

void spell3() {
    if (player.cooldowns[2] <= 0.0) {
        ScatterShot *shot;
        float rot = -M_PI / 6;

        for (int i = 0; i < 5; i++){
            Light *light = new Light;
            lights.push_back(light);

            particleSpells.push_back(new ScatterShot());

            shot = dynamic_cast<ScatterShot *>(particleSpells[particleSpells.size() - 1]);
            shot->setLight(light);
            shot->init(defferedShader, &scatter, player.getCenter() + Eigen::Vector3f(0.0f, 0.6f, 0.0f), 0, 0, 0, 0.2, true);
            Eigen::Vector3f rotation = Eigen::Vector3f(sin((float)player.getRot()(1) + rot), 0.0f, cos((float)player.getRot()(1) + rot));
            rot += M_PI / 12;
            shot->setVel(10.0f * rotation);
            shot->setReference(movingObjects);
            movingObjects.push_back(shot);
        }
        assert(fmod_system->playSound(FMOD_CHANNEL_FREE, spell3_sound, false, &channel3) == FMOD_OK);
        player.cooldowns[2] = 4.0;
    }
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods){
    //fire selected spell
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        spell1();
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        spell3();
    }

}


void intro_scene_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == ' ') {
        start = true;
        assert(channel0->setPaused(true) == FMOD_OK);
        assert(fmod_system->playSound(FMOD_CHANNEL_FREE, background, false, &channel0)== FMOD_OK);
        assert(channel0->setVolume(0.7) == FMOD_OK);
    }
}



//key callback stuff. Really should push more things into here.
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_M) {
        player.takeDamage(0.05f);
    }
    //create a new fireball
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        spell1();
    }
    //create a new arcane explosion
    else if ((key == GLFW_KEY_2 && action == GLFW_PRESS) || (key == GLFW_KEY_SPACE && action == GLFW_PRESS))
    {
        spell2();
    }

    else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        spell3();
    }


    if (key == GLFW_KEY_T && action == GLFW_PRESS){
        drawQuadTree = !drawQuadTree;
    }

    if (key == GLFW_KEY_Z && action == GLFW_PRESS){
        defferedMode = 1;
    }
    else if (key == GLFW_KEY_X && action == GLFW_PRESS){
        defferedMode = 2;
    }
    else if (key == GLFW_KEY_C && action == GLFW_PRESS){
        defferedMode = 3;
    }
    else if (key == GLFW_KEY_V && action == GLFW_PRESS){
        defferedMode = 0;
    }

}


void windowSizeCallback(long window, int w, int h)
{
    // Set view size
    width = w;
    height = h;
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    camera.setAspect(width, height);
}

void loadScene()
{
    camera.setPosition(Eigen::Vector3f(0.0, 1.5, 6.0));
    keyToggles['c'] = true;
    defferedShader.setShaderNames("Shaders/Deffered_vert.glsl", "Shaders/Deffered_frag.glsl");
    shadow.setShaderNames("Shaders/shadow_vert.glsl", "Shaders/shadow_frag.glsl");
    HudShader.setShaderNames("Shaders/hud_vert.glsl", "Shaders/hud_frag.glsl");
    fireballShader.setShaderNames("Shaders/particle_vert.glsl", "Shaders/particle_frag.glsl");

    //torranceShader.setShaderNames("Shaders/Cook_Torrance_vert.glsl", "Shaders/Cook_Torrance_frag.glsl");

    bunny.load("Assets/bunny.obj");
    sphere.load("Assets/sphereTex.obj");
    scatter.load("Assets/sphereTex.obj");
    quadrant.load("Assets/cube.obj");

    sphere.setTexture("Assets/Textures/sparks.png");
    sphere.setAlpha(true);
    scatter.setTexture("Assets/Textures/scattershot.png");
    scatter.setAlpha(true);



    scene.loadFile("source/level1.ST");

    gamehud.load(width, height);

    player.init(Eigen::Vector3f(0.0f, 0.0f, 3.0f), 0.0f, 0.0f, 0.0f, .3f, true);

    menuScreen.loadScene();
    scatter.setMat(MAT_RED);
}


void initGL()
{
    //////////////////////////////////////////////////////
    // Initialize GL for the whole scene
    //////////////////////////////////////////////////////
    //audio
    assert(FMOD::System_Create(&fmod_system) == FMOD_OK);
    assert(fmod_system->setOutput(FMOD_OUTPUTTYPE_ALSA) == FMOD_OK);
    assert(fmod_system->init(100, FMOD_INIT_NORMAL, 0) == FMOD_OK);
    assert(fmod_system->createSound("Audio/Spells/PcSkillM-fire boomerang1.wav", FMOD_2D | FMOD_DEFAULT, 0, &spell1_sound) == FMOD_OK);
    assert(fmod_system->createSound("Audio/Spells/PcSkillM-windmagic.wav", FMOD_2D | FMOD_DEFAULT, 0, &spell2_sound) == FMOD_OK);
    assert(fmod_system->createSound("Audio/Spells/PcSkillD-Gvurtial.wav", FMOD_2D | FMOD_DEFAULT, 0, &spell3_sound) == FMOD_OK);
    assert(fmod_system->createSound("Audio/Lose/BgmNPCSad.mp3", FMOD_2D | FMOD_LOOP_NORMAL, 0, &menu_song) == FMOD_OK);
    assert(fmod_system->createSound("Audio/Battle/BgmInBoss.mp3", FMOD_2D | FMOD_LOOP_NORMAL, 0, &LossSound) == FMOD_OK);
    assert(fmod_system->createSound("Audio/Consort for Brass.mp3", FMOD_2D | FMOD_LOOP_NORMAL, 0, &winSound) == FMOD_OK);
    assert(fmod_system->createSound("Audio/Battle/04 - Archangel (Choir).mp3", FMOD_2D | FMOD_LOOP_NORMAL, 0, &background) == FMOD_OK);
    assert(fmod_system->playSound(FMOD_CHANNEL_FREE, menu_song, true, &channel0) == FMOD_OK);



    // Set back colo
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // Enable z-buffer test
    glEnable(GL_DEPTH_TEST);

    ////////////////////////////////////////////////////
    ////Initialize lights
    ///////////////////////////////////////////////////
    shadowLight.setPosition(Eigen::Vector3f(0.0, 5.0, 0.0));

    Light *light = new Light;
    light->setPosition(Eigen::Vector3f(0.0, 5.0, 0.0));
    light->setAmbience(Eigen::Vector3f(0.1, 0.1, 0.1));
    light->setLuminance(Eigen::Vector3f(0.2, 0.3, 0.4));
    light->setAtten(0.001, 0.001, 0.001);
    lights.push_back(light);
    light = new Light;
    light->setPosition(Eigen::Vector3f(0.0, 3.0, 10.0));
    light->setAmbience(Eigen::Vector3f(0.1, 0.1, 0.1));
    light->setLuminance(2 *Eigen::Vector3f(2.0, 1.4, .1));
    light->setAtten(0.1, 0.1, 0.1);
    lights.push_back(light);
    light = new Light;
    light->setPosition(Eigen::Vector3f(20.0, 3.0, 8.0));
    light->setAmbience(Eigen::Vector3f(0.1, 0.1, 0.1));
    light->setLuminance(2 *Eigen::Vector3f(2.0, 1.4, .1));
    light->setAtten(0.1, 0.1, 0.1);
    lights.push_back(light);
    light = new Light;
    light->setPosition(Eigen::Vector3f(-20.0, 3.0, 3.0));
    light->setAmbience(Eigen::Vector3f(0.1, 0.1, 0.1));
    light->setLuminance(2 *Eigen::Vector3f(2.0, 1.4, .1));
    light->setAtten(0.1, 0.1, 0.1);
    lights.push_back(light);
    light = new Light;
    light->setPosition(Eigen::Vector3f(-3.0, 3.0, -6.0));
    light->setAmbience(Eigen::Vector3f(0.1, 0.1, 0.1));
    light->setLuminance(2 *Eigen::Vector3f(2.0, 1.4, .1));
    light->setAtten(0.1, 0.1, 0.1);
    lights.push_back(light);
    light = new Light;
    light->setPosition(Eigen::Vector3f(12.0, 3.0, -7.0));
    light->setAmbience(Eigen::Vector3f(0.1, 0.1, 0.1));
    light->setLuminance(2 *Eigen::Vector3f(2.0, 1.4, .1));
    light->setAtten(0.1, 0.1, 0.1);
    lights.push_back(light);

    //////////////////////////////////////////////////////
    // Intialize the shapes
    //////////////////////////////////////////////////////
    bunny.init();
    assert(glGetError() == GL_NO_ERROR);
    sphere.init();
    scatter.init();
    quadrant.init();
    //wizardModel.init();

    scene.init();

    gamehud.setup();

    //////////////////////////////////////////////////////
    // Intialize the shaders
    //////////////////////////////////////////////////////

    assert(fireballShader.init());
    fireballShader.addUniform("CameraRight_worldspace");
    fireballShader.addUniform("CameraUp_worldspace");
    fireballShader.addUniform("tex");
    fireballShader.addUniform("VP");
    fireballShader.addAttribute("squareVertices");
    fireballShader.addAttribute("xyzs");
    fireballShader.addAttribute("partLife");
    fireballShader.addAttribute("TexCoord");

    assert(defferedShader.init());
    defferedShader.addUniform("P");
    defferedShader.addUniform("MV");
    defferedShader.addAttribute("vertPos");
    defferedShader.addAttribute("vertNor");
    defferedShader.addAttribute("vertTex");
    defferedShader.addUniform("model");


    defferedShader.addAttribute("weights0");
    defferedShader.addAttribute("weights1");
    defferedShader.addAttribute("weights2");
    defferedShader.addAttribute("weights3");

    defferedShader.addAttribute("bones0");
    defferedShader.addAttribute("bones1");
    defferedShader.addAttribute("bones2");
    defferedShader.addAttribute("bones3");

    defferedShader.addAttribute("numWeights");
    defferedShader.addUniform("animM");
    defferedShader.addUniform("bindPose");
    defferedShader.addUniform("texture0");


    //defferedShader.addUniform("ka");
    //defferedShader.addUniform("kd");
    //defferedShader.addUniform("ks");
    //defferedShader.addUniform("s");
    //defferedShader.addUniform("inten");
    //defferedShader.addUniform("texture1");
    //defferedShader.addUniform("texture2");


    //Init hud shader
    HudShader.init();
    cout << "Initialized Hud Shader\n";
    HudShader.addUniform("P");
    HudShader.addUniform("MV");
    HudShader.addAttribute("vertPos");

    HudShader.addAttribute("vertTex");

    HudShader.addUniform("texture0");
    HudShader.addUniform("texture1");


    //Now do shadow shader
    assert(shadow.init());
    cout << "Initialized Shadow Shader\n";

    shadow.addAttribute("vertPos");
    shadow.addUniform("P");
    shadow.addUniform("MV");


    menuScreen.bindProg(&defferedShader);
    //////////////////////////////////////////////////////
    // Shadow texture stuff
    //////////////////////////////////////////////////////

    glGenFramebuffers(1, &framebufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

    glGenTextures(1, &shadowMap);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowmap_width, shadowmap_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        cerr << "Framebuffer is not ok" << endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //////////////////////////////////////////////////////
    // Environment mapping
    //////////////////////////////////////////////////////
    envShader.setShaderNames("Shaders/envmap_vert.glsl", "Shaders/envmap_frag.glsl");
    assert(envShader.init());
    envShader.addUniform("P");
    envShader.addUniform("M");
    envShader.addUniform("V");
    envShader.addAttribute("vertPos");
    envShader.addAttribute("vertNor");
    envShader.addAttribute("vertTex");
    envShader.addUniform("camLoc");
    envShader.addUniform("texture0");
    envShader.addUniform("lightWorld");

    std::vector<std::string> faces;
    faces.push_back("Assets/skybox/right.jpg");
    faces.push_back("Assets/skybox/left.jpg");
    faces.push_back("Assets/skybox/top.jpg");
    faces.push_back("Assets/skybox/bottom.jpg");
    faces.push_back("Assets/skybox/back.jpg");
    faces.push_back("Assets/skybox/front.jpg");
    env.setFaces(faces);
    env.init();

    envMapped.init(&bunny, Eigen::Vector3f(1.5, 1.5, 1.5), 0, 0, 0, 0.5, false);

    //////////////////////////////////////////////////////
    // Final check for errors
    //////////////////////////////////////////////////////
    GLSL::checkVersion();

    glfwSetScrollCallback(window, scroll_callback);

    assert(glGetError() == GL_NO_ERROR);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    //test.init(&bunny, Eigen::Vector3f(0, 0, 0), 0, 0, 0, 1, false);
    //test.initPath(Eigen::Vector2f(0, 0), &tree);
    assert(glGetError() == GL_NO_ERROR);
}


void drawGL()
{
    GLuint loc;
    // Elapsed time
    assert(fmod_system->update() == FMOD_OK);

    double tCurr, tchange; // in seconds
    double tPrev = glfwGetTime();
    timeSlice = TimeManager::Instance().DeltaTime;
    gameTime += timeSlice;
    //printf("Slice: %f\n", timeSlice);

    //fps stuff
    TimeManager::Instance().CalculateFrameRate(true);

    // Clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_CULL_FACE);

    MatrixStack P, MV, V;


    // Apply camera transforms
    P.pushMatrix();
    camera.applyProjectionMatrix(&P);
    MV.pushMatrix();
    camera.applyViewMatrix(&MV);
    V.pushMatrix();
    camera.applyViewMatrix(&V);

    //////////////////////////////////////////////////////
    // Shadow map pass
    //////////////////////////////////////////////////////

    MatrixStack lightP, lightMV; // light matrices
    shadow.bind();

    lightP.pushMatrix();
    shadowLight.applyProjectionMatrix(&lightP);

    assert(glGetError() == GL_NO_ERROR);
    glUniformMatrix4fv(shadow.getUniform("P"), 1, GL_FALSE, lightP.topMatrix().data());

    lightMV.pushMatrix();
    shadowLight.applyViewMatrix(&lightMV);
    glUniformMatrix4fv(shadow.getUniform("MV"), 1, GL_FALSE, lightMV.topMatrix().data());

    Eigen::Matrix4f lightMVP = lightP.topMatrix() * lightMV.topMatrix();

    // Get light position in camera space
    Eigen::Vector4f lightPos = MV.topMatrix() * Eigen::Vector4f(shadowLight.getPosition().x(), shadowLight.getPosition().y(), shadowLight.getPosition().z(), 1.0);


    //Create shadow buffer

    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
    glViewport(0, 0, shadowmap_width, shadowmap_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);

    scatter.bindProg(&shadow);
    bunny.bindProg(&shadow);
    sphere.bindProg(&shadow);
    wizardModel.bindProg(&shadow);
    scene.bindProg(&shadow);
    player.bindProg(&shadow);

    player.shadowDraw(&MV);
    scene.shadowDraw(&MV);

    for (int index = 0; index < movingObjects.size(); index++){
        movingObjects[index]->shadowDraw(&lightMV);
    }

    shadow.unbind();
    if (debugPrint == true){
        tCurr = glfwGetTime();
        tchange = (tCurr - tPrev);
        tPrev = tCurr;
        printf("Shadow pass time:   %f\n", tchange);
    }
    assert(glGetError() == GL_NO_ERROR);

    //////////////////////////////////////////////////////
    // Draw pass
    //////////////////////////////////////////////////////
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_BACK);

    scatter.bindProg(&defferedShader);

    if (SHADER_DEBUG == 1)
    {
        bunny.bindProg(&defferedShader);
        sphere.bindProg(&defferedShader);
        quadrant.bindProg(&defferedShader);
        wizardModel.bindProg(&defferedShader);
        scene.bindProg(&defferedShader);
        defferedShader.bind();



        glUniformMatrix4fv(defferedShader.getUniform("P"), 1, GL_FALSE, P.topMatrix().data());
        glUniformMatrix4fv(defferedShader.getUniform("MV"), 1, GL_FALSE, MV.topMatrix().data());
        def_shader.start();
    }
    wizardModel.bindProg(&defferedShader);

    player.bindProg(&defferedShader);
    player.draw(&MV);
    scene.draw(&MV);



    for (int index = 0; index < movingObjects.size(); index++){
        GameObject *object = dynamic_cast<FireBall *> (movingObjects[index]);
        if (object != NULL) {
            continue;
        }
            assert(glGetError() == GL_NO_ERROR);
            movingObjects[index]->draw(&MV);
            assert(glGetError() == GL_NO_ERROR);
            //movingObjects[index]->drawBounding(&MV, &sphere);
           
             if (movingObjects[index]->shouldRemoveFromWorld())
            {
                //possible memory leak? include destructor for object?
                movingObjects.erase(movingObjects.begin() + index);
                //to avoid skipping the next in the vector?
                index--;
            }

    }

    if (drawQuadTree){
        tree.drawQuadTree(&MV);
    }

    //test.draw(&MV);
    //sphere.setWireframe(true);
    //scene.drawBounding(&MV, &sphere);
    //player.drawBounding(&MV, &sphere);
    //sphere.setWireframe(false);
    if (debugPrint == true){
        tCurr = glfwGetTime();
        tchange = (tCurr - tPrev);
        tPrev = tCurr;
        printf("Deferred time:     %f\n", tchange);
    }

    //Environmental mapping
    //Bind shapes
    bunny.bindProg(&envShader);
    envShader.bind();

    glUniform3fv(envShader.getUniform("lightWorld"), 1, Eigen::Vector3f(0, 1, 0).data());
    glUniformMatrix4fv(envShader.getUniform("P"), 1, GL_FALSE, P.topMatrix().data());
    glUniformMatrix4fv(envShader.getUniform("V"), 1, GL_FALSE, V.topMatrix().data());
    glUniform3fv(envShader.getUniform("camLoc"), 1, camera.getPos().data());
    assert(glGetError() == GL_NO_ERROR);
    envMapped.drawEnv(&MV, env);

    envShader.unbind();


    //Particle stuff
    //We get to keep our jobs because no one will be able to maintain this code. #refuctoring
    for (int ndx = 0; ndx < particleSpells.size(); ndx++)
    {
        GameObject *object = dynamic_cast<ScatterShot *>(particleSpells[ndx]);
        if (object == NULL) {
            particleSpells[ndx]->update(timeSlice, tree);
            particleSpells[ndx]->draw(&MV);
        }

        if (particleSpells[ndx]->getLife() <= 0.0f)
        {
            //possible memory leak? include destructor for object?
            lights.erase(lights.begin() + 6 + ndx);
            particleSpells.erase(particleSpells.begin() + ndx);
            //to avoid skipping the next in the vector?
            ndx--;
        }
    }
    if (SHADER_DEBUG == 1) {


        defferedShader.unbind();
        def_shader.bind();
        def_shader.setLightData(lights);
        def_shader.setShadowData(shadowMap, lightMVP, shadowMap);
        def_shader.render(defferedMode, camera.getPos(), lights.size());
    }

    if (debugPrint == true){
        tCurr = glfwGetTime();
        tchange = (tCurr - tPrev);
        tPrev = tCurr;
        printf("Particle time:     %f\n", tchange);
    }

    assert(glGetError() == GL_NO_ERROR);
    //////////////////////////////////////////////////////
    // Game Engine
    //////////////////////////////////////////////////////
    for (int index = 0; index < movingObjects.size(); index++){
        GameObject *object = dynamic_cast<FireBall *> (movingObjects[index]);
        if (object == NULL) {

            //Call proper update depending on type.
            if (Bookmonster *e = dynamic_cast<Bookmonster *> (movingObjects[index])){
                e->update(timeSlice, tree);
                e->spellCallback = &EnemySpellCastFunc;
            }
            else if (Enemy *e = dynamic_cast<Enemy *> (movingObjects[index])){
                e->update(timeSlice, tree);
            }
            else if (Spell *s = dynamic_cast<Spell *> (movingObjects[index])){
                s->update(timeSlice, tree);
            }
            else if (EnemySpell *a = dynamic_cast<EnemySpell *> (movingObjects[index])){
               a->update(timeSlice, tree);
            }
        }
    }


    tree.clear();
    for (int index = 0; index < movingObjects.size(); index++){
        tree.insert(movingObjects[index]);
    }
    //Use tree to hold static objects
    scene.populateTree(&tree);
    //And dynamic ones
    tree.insert(&player);

    defferedShader.bind();
    player.update(window, timeSlice, width, height, tree, camera.getWorldPos(), &MV, &P, &V);
    defferedShader.unbind();
    //test.update(timeSlice);
    //printf("player: x:%f y:%f z:%f\n", player.getPos().x(), player.getPos().y(), player.getPos().z());
    spawner.update(gameTime, &movingObjects);
    //printf("num moving objects: %d\n", movingObjects.size());


    if (debugPrint == true){
        tCurr = glfwGetTime();
        tchange = (tCurr - tPrev);
        tPrev = tCurr;
        printf("Game engine time:   %f\n\n", tchange);
    }

    // Pop stacks
    MV.popMatrix();
    P.popMatrix();
    V.popMatrix();
    gamehud.draw(&P, &MV, &player, gameTime, window);
    if (player.getLife() <= 0 && gameState == 0)
    {
        gameState = 1;

        assert(channel0->setPaused(true) == FMOD_OK);
        assert(fmod_system->playSound(FMOD_CHANNEL_FREE, LossSound, false, &channel0)== FMOD_OK);
    }
    if (gameTime >= 320 && gameState == 0)
    {
        gameState = 1;
        assert(channel0->setPaused(true) == FMOD_OK);
        assert(fmod_system->playSound(FMOD_CHANNEL_FREE, winSound, false, &channel0)== FMOD_OK);
    }
    assert(glGetError() == GL_NO_ERROR);
}

// Function called during loop to spawn a new enemy at a set interval
void spawnEnemies(){
    spawnTime += timeSlice;
    if (spawnTime > ENEMY_INTERVAL){
        spawnTime = 0.0;
        float bound = GROUND_SIZE / 2 - GROUND_SIZE*.05;
        Eigen::Vector3f pos = Eigen::Vector3f(randFloat(-bound, bound), 0, randFloat(-bound, bound));
        movingObjects.push_back(new Enemy());
        Enemy *enemy = dynamic_cast<Enemy *>(movingObjects[movingObjects.size() - 1]);
        enemy->init(&bunny, pos, 0, 0, 0, 0.5, true);
        enemy->setReferences(&player, &scene);
    }
}

void restartGame() {
    gameState = 0;

    for (int i = 0; i < movingObjects.size(); i++) {
        movingObjects.erase(movingObjects.begin() + i);
        i--;
    }
    player.restart(Eigen::Vector3f(0.0f, 0.0f, 3.0f));
    spawner.populate(&player, &scene);
    gameTime = 0;
    TimeManager::Instance().CalculateFrameRate(false);
    TimeManager::Instance().CalculateFrameRate(false);
}


int main(int argc, char **argv)
{
    bool fullscreen = true;

    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Set OpenGL major version to 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Set OpenGL minor version to 3

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  //not sure if necessary, but post said sometimes prevented cross-machine bugs
    glfwWindowHint(GLFW_SAMPLES, 4); //Set number of samples to use for multisampling, 4 is what Zoe had
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    int winWidth = mode->width;
    int winHeight = mode->height;

    std::string windowName = "Armarium";

    if (fullscreen) {
        window = glfwCreateWindow(winWidth, winHeight, windowName.c_str(), glfwGetPrimaryMonitor(), NULL);
    }
    else {
        winWidth -= 200;
        winHeight -= 200;
        window = glfwCreateWindow(winWidth, winHeight, windowName.c_str(), NULL, NULL);
    }
    camera.setAspect(winWidth, winHeight);
    if (window == NULL){
        fprintf(stderr, "Failed to open GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); //check if necessary or happens by default, docs unclear

    //glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE); //not sure if necessary, Zoe had comment on it ensuring capture of the escape key. 
    // The docs explain that it keep the pollable state of a key constant until polled.
    // http://www.glfw.org/docs/latest/group__input.html#gaa92336e173da9c8834558b54ee80563b


    if (!gladLoadGL()) {
        fprintf(stderr, "Unable to initialize glad");
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    windowSizeCallback(0, winWidth, winHeight);
    GLFWwindowsizefun callback = GLFWwindowsizefun(windowSizeCallback);
    glfwSetWindowSizeCallback(window, callback);
    loadScene();
    printf("OpenGl version %s GLSL version %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
    initGL();




    def_shader.init(winWidth, winHeight);
    do {
        glfwSetKeyCallback(window, intro_scene_key_callback);
        assert(channel0->setPaused(true) == FMOD_OK);
        assert(fmod_system->playSound(FMOD_CHANNEL_FREE, menu_song, false, &channel0)== FMOD_OK);
        do {
            menuScreen.update(.01);
            menuScreen.draw();
            glfwSwapBuffers(window);
            glfwPollEvents();
        } while (start == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && glfwWindowShouldClose(window) != 0){
            break;
        }
        start = false;
        glfwSetKeyCallback(window, key_callback);


        TimeManager::Instance().CurrentTime = 0.0f;
        //TimeManager::Instance().Last
        //TimeManager::Instance().DeltaTime;
        //Game engine init
        camera.followObject(&player);
        quadrant.setWireframe(true);
        tree.setShape(&quadrant);

        //Add shapes to be referenced by enemies.
        spawner.addShape(&bunny);
        restartGame();

        // Now set the callback
        do{
            drawGL();
            //spawnEnemies(); //bunnies
            if (camera.active) {
                camera.update(window, timeSlice);
            }
            else {
                camera.followObject(&player);
            }
            // Swap buffers
            glfwSwapBuffers(window);
            glfwPollEvents();

        } while (!glfwGetKey(window, GLFW_KEY_R) &&
            glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    glfwTerminate();
    return 0;
}
