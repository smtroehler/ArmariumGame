#pragma  once
#ifndef __DEF_SHADER__
#define __DEF_SHADER__


#include "GLSL.h"
#include "GameObject.h"
#include "MatrixStack.h"
#include "Program.h"
#include "Light.h"
#include "../dependencies/Eigen/Dense"
class DefferedShader
{
public:
	DefferedShader();
    ~DefferedShader();
    void init(int width, int height);
    //void draw(GameObject *obj, MatrixStack *mv);
    void bind();
    void start();
    void setLightData(std::vector<Light*> lights);
    void setShadowData(GLint shadowMap, Eigen::Matrix4f lightMVP, GLint shadowmap);
    void render(int mode, Eigen::Vector3f camera, int numLights);
private:
	Program prog;
    GLuint colorBuffer = 0;
    GLuint renderedTexture;
    GLuint norBuffer = 0;
    GLuint norTexture;
    GLuint posTexture;
    GLuint quad_VertexArrayID;
    GLuint quad_vertexbuffer;
    int width;
    int height;
};

#endif
