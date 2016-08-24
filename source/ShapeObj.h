#pragma once
#ifndef _SHAPEOBJ_H_
#define _SHAPEOBJ_H_
#define EIGEN_DONT_VECTORIZE
#define EIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT
#define EIGEN_DONT_ALIGN

#include "tiny_obj_loader.h"
#include "Program.h"
#include "MatrixStack.h"
#include "Texture.h"
#include <string>
#include "ImportSmd.h"
#include "CubeMap.h"

//List of material names  
#define MAT_BLUE 0
#define MAT_WHITE 1 
#define MAT_RED 2
#define MAT_GREEN 3
#define MAT_PURPLE 4

enum FileType {
      SMD = 0,
      OBJ
};   

class ShapeObj
{
public:
    ShapeObj();
    virtual ~ShapeObj();
    void loadSmd(const std::string &filename);
    void load(const std::string &meshName);
    void init();

    void bindProg(Program *toBind);
    void draw(MatrixStack* MV);
    void shadowDraw(MatrixStack* MV);
    void drawEnv(MatrixStack* MV, CubeMap env);
    
    void applyMat();
    void setTexture(std::string filename) { texture.setFilename(filename); }
    void setLightmap(std::string filename) { lightmap.setFilename(filename); }
    void setBumpmap(std::string filename) { bumpmap.setFilename(filename); }
    void setMat(int m) { material = m; };
    void setWireframe(bool val) { wireframe = val; }
    void setAlpha(bool alpha) { useAlpha = alpha; }

   

    Eigen::Vector3f getCenter() { return center; }
    float getxHalf() { return xHalf; }
    float getyHalf() { return yHalf; }
    float getzHalf() { return zHalf; }
    float getRadius() { return radius; }
    float getRadius3D() { return radius3D; }

    std::string fileName;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<importsmd::shape_smd> smd_shapes;


    void loadSmdAnim(const std::string &filename);
   
    int getNumFrames(int animationIndex);
    void animate( int frame, int animation);
    void AnimationToMatrix(int anim_index);
    std::vector<Eigen::Matrix4f> getFrame(importsmd::keyframe kf);
    void BindPoseToMatrix();
    Eigen::Matrix4f getTotalMatforFrame(int joint, importsmd::keyframe kf, std::vector<int> parents);
    int curFrame = 0;
protected:
    bool animated = false;
    bool useAlpha;
    FileType filetype;
    Texture texture, lightmap, bumpmap;
    int material;
    unsigned posBufID, norBufID, texBufID, indBufID;
    float xmin, ymin, zmin, xmax, ymax, zmax;
    float xHalf, yHalf, zHalf, radius, radius3D;
    bool wireframe;
    Eigen::Vector3f center;
    GLuint vao;
    Program *prog;

    //animation stuff


    std::vector<float> weightBuf;
    std::vector<float> boneBuf;
    std::vector<float> numWeightsBuf;

    std::vector<Eigen::Matrix4f> animM;
    std::vector<float> animVec;
    std::vector<float> bindVec;

    std::vector<std::vector< std::vector<Eigen::Matrix4f>> >transformFrames;
    std::vector<Eigen::Matrix4f> bindPose;

    unsigned boneBufID;
    unsigned weightBufID;
    unsigned numWeightsBufID;
};

#endif
