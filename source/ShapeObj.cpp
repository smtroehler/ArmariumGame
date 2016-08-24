#define EIGEN_DONT_VECTORIZE
#define EIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT
#define EIGEN_DONT_ALIGN

#include <iostream>
#include "../dependencies/Eigen/Dense"
#include "ShapeObj.h"
#include "GLSL.h"
#include <limits>
#include <cfloat>



using namespace std;

ShapeObj::ShapeObj() :
posBufID(0),
norBufID(0),
texBufID(0),
indBufID(0),
xmin(FLT_MAX),
ymin(FLT_MAX),
zmin(FLT_MAX),
xmax(-FLT_MAX),
ymax(-FLT_MAX),
zmax(-FLT_MAX),
wireframe(false),
useAlpha(false)
{
    texture.setFilename("Assets/Textures/missing_texture.jpg");
   //lightmap.setFilename("Assets/Textures/missing_lightmap.jpg"); 
   //bumpmap.setFilename("Assets/Textures/missing_bumpmap.jpg"); 
}

ShapeObj::~ShapeObj()
{
}

void ShapeObj::loadSmd(const std::string &filename) {
   filetype = SMD;
   importsmd::LoadSmd(smd_shapes, filename.c_str());
   float x, y, z;
    for (int i = 0; i < smd_shapes[0].mesh.positions.size() / 3; i++) {
        x = smd_shapes[0].mesh.positions[3 * i + 0];
        y = smd_shapes[0].mesh.positions[3 * i + 1];
        z = smd_shapes[0].mesh.positions[3 * i + 2];
        if (x > xmax)
            xmax = x;
        if (y > ymax)
            ymax = y;
        if (z > zmax)
            zmax = z;
        if (x < xmin)
            xmin = x;
        if (y < ymin)
            ymin = y;
        if (z < zmin)
            zmin = z;
    }
    center = Eigen::Vector3f((xmin + xmax) / 2.f, (ymin + ymax) / 2.f, (zmin + zmax) / 2.f);
    xHalf = xmax - center.x();
    yHalf = ymax - center.y();
    zHalf = zmax - center.z();
    radius = sqrt(pow(xHalf, 2) + pow(zHalf, 2)); //only x-z (circle)
    radius3D = sqrt(pow(xHalf, 2) + pow(yHalf, 2) + pow(zHalf, 2)); //x,y,z (sphere)
    fileName = filename;

}


void ShapeObj::load(const string &meshName)
{
    filetype = OBJ;
    vector<tinyobj::material_t> objMaterials;
    string err = tinyobj::LoadObj(shapes, objMaterials, meshName.c_str());
    if (!err.empty()) {
        cerr << err << endl;
    }
    else {
        float x, y, z;
        for (int i = 0; i < shapes[0].mesh.positions.size() / 3; i++) {
            x = shapes[0].mesh.positions[3 * i + 0];
            y = shapes[0].mesh.positions[3 * i + 1];
            z = shapes[0].mesh.positions[3 * i + 2];
            if (x > xmax)
                xmax = x;
            if (y > ymax)
                ymax = y;
            if (z > zmax)
                zmax = z;
            if (x < xmin)
                xmin = x;
            if (y < ymin)
                ymin = y;
            if (z < zmin)
                zmin = z;
        }
        center = Eigen::Vector3f((xmin + xmax) / 2.f, (ymin + ymax) / 2.f, (zmin + zmax) / 2.f);
        xHalf = xmax - center.x();
        yHalf = ymax - center.y();
        zHalf = zmax - center.z();
        radius = sqrt(pow(xHalf, 2) + pow(zHalf, 2)); //only x-z (circle)
        radius3D = sqrt(pow(xHalf, 2) + pow(yHalf, 2) + pow(zHalf, 2)); //x,y,z (sphere)
        fileName = meshName;
    }
}


void ShapeObj::init()
{
   texture.init(useAlpha);
   //lightmap.init(false);
   //bumpmap.init(false);
   
   // Send the position array to the GPU
    vector<float> posBuf;
   if(filetype == OBJ)
      posBuf = shapes[0].mesh.positions;
   else if(filetype == SMD)
      posBuf = smd_shapes[0].mesh.positions;

   glGenBuffers(1, &posBufID);
   glBindBuffer(GL_ARRAY_BUFFER, posBufID);
   glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);

   glGenVertexArrays(1, &vao);
   glBindVertexArray(vao);
   
   // Send the normal array (if it exists) to the GPU
   vector<float> norBuf;
   if(filetype == OBJ)
      norBuf = shapes[0].mesh.normals;
   else if(filetype == SMD) 
      norBuf = smd_shapes[0].mesh.normals;

   if(!norBuf.empty()) {
      glGenBuffers(1, &norBufID);
      glBindBuffer(GL_ARRAY_BUFFER, norBufID);
      glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_STATIC_DRAW);
   } else {
      norBufID = 0;
   }
   
   // Send the texture coordinates array (if it exists) to the GPU
   vector<float> texBuf;
   if(filetype == OBJ)
      texBuf = shapes[0].mesh.texcoords;
   else if(filetype == SMD)
      texBuf = smd_shapes[0].mesh.texcoords;

   if(!texBuf.empty()) {
      glGenBuffers(1, &texBufID);
      glBindBuffer(GL_ARRAY_BUFFER, texBufID);
      glBufferData(GL_ARRAY_BUFFER, texBuf.size()*sizeof(float), &texBuf[0], GL_STATIC_DRAW);
   } else {
      texBufID = 0;
   }

   // Send the index array to the GPU
   vector<unsigned int> indBuf;
   if(filetype == OBJ) 
      indBuf = shapes[0].mesh.indices;
   else if(filetype == SMD)
      indBuf = smd_shapes[0].mesh.indices;

   glGenBuffers(1, &indBufID);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufID);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, indBuf.size()*sizeof(unsigned int), &indBuf[0], GL_STATIC_DRAW);
   

    if(animated) 
    {
        glGenBuffers(1, &boneBufID);
        glBindBuffer(GL_ARRAY_BUFFER, boneBufID);
        glBufferData(GL_ARRAY_BUFFER, boneBuf.size()*sizeof(float), &boneBuf[0],GL_STATIC_DRAW);

        glGenBuffers(1, &weightBufID);
        glBindBuffer(GL_ARRAY_BUFFER, weightBufID);
        glBufferData(GL_ARRAY_BUFFER, weightBuf.size()*sizeof(float), &weightBuf[0],GL_STATIC_DRAW);


        std::vector<int> numJointsAtVertex = smd_shapes[0].mesh.numbonesbinded;
        glGenBuffers(1, &numWeightsBufID);
        glBindBuffer(GL_ARRAY_BUFFER, numWeightsBufID);
        glBufferData(GL_ARRAY_BUFFER, smd_shapes[0].mesh.numbonesbinded.size()*sizeof(int), &(smd_shapes[0].mesh.numbonesbinded[0]),GL_STATIC_DRAW);
    }
    else {
        std::vector<int> numJointsAtVertex;
        for(int i = 0; i < indBuf.size(); i++)
            numJointsAtVertex.push_back(0);

        glGenBuffers(1, &numWeightsBufID);
        glBindBuffer(GL_ARRAY_BUFFER, numWeightsBufID);
        glBufferData(GL_ARRAY_BUFFER, numJointsAtVertex.size()*sizeof(int), &(numJointsAtVertex[0]),GL_STATIC_DRAW);
    }


   // Unbind the arrays
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   
   assert(glGetError() == GL_NO_ERROR);
}

void ShapeObj::bindProg(Program *toBind) {
    prog = toBind;
}

void ShapeObj::applyMat(){
    switch (material){
    case MAT_WHITE:
        glUniform3fv(prog->getUniform("ka"), 1, Eigen::Vector3f(0.1, 0.1, 0.1).data());
        //glUniform3fv(prog->getUniform("kd"), 1, Eigen::Vector3f(0.8, 0.8, 0.8).data());
        //glUniform3fv(prog->getUniform("ks"), 1, Eigen::Vector3f(.3, .3, .3).data());
        glUniform1f(prog->getUniform("s"), 50.0);
        break;
    case MAT_BLUE:
        glUniform3fv(prog->getUniform("ka"), 1, Eigen::Vector3f(0.1, 0.1, 0.1).data());
        //glUniform3fv(prog->getUniform("kd"), 1, Eigen::Vector3f(0.2, 0.2, 0.8).data());
        //glUniform3fv(prog->getUniform("ks"), 1, Eigen::Vector3f(.3, .3, .3).data());
        glUniform1f(prog->getUniform("s"), 20.0);
        break;
    case MAT_RED:
        glUniform3fv(prog->getUniform("ka"), 1, Eigen::Vector3f(0.1, 0.1, 0.1).data());
        //glUniform3fv(prog->getUniform("kd"), 1, Eigen::Vector3f(0.8, 0.2, 0.2).data());
        //glUniform3fv(prog->getUniform("ks"), 1, Eigen::Vector3f(.3, .3, .3).data());
        glUniform1f(prog->getUniform("s"), 20.0);
        break;
    case MAT_GREEN:
        glUniform3fv(prog->getUniform("ka"), 1, Eigen::Vector3f(0.1, 0.1, 0.1).data());
        //glUniform3fv(prog->getUniform("kd"), 1, Eigen::Vector3f(0.2, 0.8, 0.2).data());
        //glUniform3fv(prog->getUniform("ks"), 1, Eigen::Vector3f(.3, .3, .3).data());
        glUniform1f(prog->getUniform("s"), 20.0);
        break;
    case MAT_PURPLE:
        glUniform3fv(prog->getUniform("ka"), 1, Eigen::Vector3f(0.1, 0.1, 0.1).data());
        //glUniform3fv(prog->getUniform("kd"), 1, Eigen::Vector3f(0.5, 0.2, 0.8).data());
        //glUniform3fv(prog->getUniform("ks"), 1, Eigen::Vector3f(.3, .3, .3).data());
        glUniform1f(prog->getUniform("s"), 20.0);
        break;
    }
}

void ShapeObj::shadowDraw(MatrixStack* MV){
    int h_pos = prog->getAttribute("vertPos");
    glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, MV->topMatrix().data());

    // Enable and bind position array for drawing
    GLSL::enableVertexAttribArray(h_pos);
    glBindBuffer(GL_ARRAY_BUFFER, posBufID);
    glVertexAttribPointer(h_pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Bind index array for drawing
    int nIndices;
   if(filetype == OBJ)
      nIndices = (int)shapes[0].mesh.indices.size();
   else
      nIndices = (int)smd_shapes[0].mesh.indices.size();
   
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufID);
    //assert(glGetError() == GL_NO_ERROR);
    // Draw
    glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

    // Disable and unbind
    GLSL::disableVertexAttribArray(h_pos);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ShapeObj::drawEnv(MatrixStack* MV, CubeMap env){
    env.bind(prog->getUniform("texture0"), 1);

    glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, MV->topMatrix().data());
    int h_pos = prog->getAttribute("vertPos");

    // Enable and bind position array for drawing
    GLSL::enableVertexAttribArray(h_pos);

    assert(glGetError() == GL_NO_ERROR);
    glBindBuffer(GL_ARRAY_BUFFER, posBufID);

    assert(glGetError() == GL_NO_ERROR);
    glVertexAttribPointer(h_pos, 3, GL_FLOAT, GL_FALSE, 0, 0);
    assert(glGetError() == GL_NO_ERROR);
    int h_nor = prog->getAttribute("vertNor");
    // Enable and bind normal array (if it exists) for drawing
    if (norBufID && h_nor >= 0) {
        GLSL::enableVertexAttribArray(h_nor);
        glBindBuffer(GL_ARRAY_BUFFER, norBufID);
        glVertexAttribPointer(h_nor, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }

    // Enable and bind texcoord array (if it exists) for drawing

    int h_tex = prog->getAttribute("vertTex");
    if (texBufID && h_tex >= 0) {
        GLSL::enableVertexAttribArray(h_tex);
        glBindBuffer(GL_ARRAY_BUFFER, texBufID);
        glVertexAttribPointer(h_tex, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }

    // Bind index array for drawing
    int nIndices;
    nIndices = (int)shapes[0].mesh.indices.size();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufID);

    // Draw
    glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

    // Disable and unbind
    if (texBufID && h_tex >= 0) {
        GLSL::disableVertexAttribArray(h_tex);
    }
    if (norBufID && h_nor >= 0) {
        GLSL::disableVertexAttribArray(h_nor);
    }
    GLSL::disableVertexAttribArray(h_pos);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    env.unbind();
    assert(glGetError() == GL_NO_ERROR);
}


void ShapeObj::draw(MatrixStack* MV)
{
    
    // ERROR TEXTURE PROBLEM STARTS HERE
    
    texture.bind(prog->getUniform("texture0"), 0);
    //lightmap.bind(prog->getUniform("texture1"), 2);
    //bumpmap.bind(prog->getUniform("texture2"), 3);
   glUniformMatrix4fv(prog->getUniform("model"), 1, GL_FALSE, MV->topMatrix().data());
   MV->popMatrix();
    glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, MV->topMatrix().data());

    int h_pos = prog->getAttribute("vertPos");

    // Enable and bind position array for drawing
    GLSL::enableVertexAttribArray(h_pos);

    assert(glGetError() == GL_NO_ERROR);
    glBindBuffer(GL_ARRAY_BUFFER, posBufID);

    assert(glGetError() == GL_NO_ERROR);
    glVertexAttribPointer(h_pos, 3, GL_FLOAT, GL_FALSE, 0, 0);
    assert(glGetError() == GL_NO_ERROR);
    int h_nor = prog->getAttribute("vertNor");
    // Enable and bind normal array (if it exists) for drawing
    if (norBufID && h_nor >= 0) {
        GLSL::enableVertexAttribArray(h_nor);
        glBindBuffer(GL_ARRAY_BUFFER, norBufID);
        glVertexAttribPointer(h_nor, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }

    // Enable and bind texcoord array (if it exists) for drawing

    int h_tex = prog->getAttribute("vertTex");
    if (texBufID && h_tex >= 0) {
        GLSL::enableVertexAttribArray(h_tex);
        glBindBuffer(GL_ARRAY_BUFFER, texBufID);
        glVertexAttribPointer(h_tex, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }


    // Bind index array for drawing
    int nIndices;
    if(filetype == OBJ)
        nIndices = (int)shapes[0].mesh.indices.size();
    else
        nIndices = (int)smd_shapes[0].mesh.indices.size();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufID);

    if(animated) {
        GLSL::enableVertexAttribArray(prog->getAttribute("weights0"));
        GLSL::enableVertexAttribArray(prog->getAttribute("weights1"));
        GLSL::enableVertexAttribArray(prog->getAttribute("weights2"));
        GLSL::enableVertexAttribArray(prog->getAttribute("weights3"));
        glBindBuffer(GL_ARRAY_BUFFER, weightBufID);

        unsigned stride = 16*sizeof(float);
        glVertexAttribPointer(prog->getAttribute("weights0"), 4, GL_FLOAT, GL_FALSE,stride, (const void *)( 0*sizeof(float)));
        glVertexAttribPointer(prog->getAttribute("weights1"), 4, GL_FLOAT, GL_FALSE, stride, (const void *)( 4*sizeof(float)));
        glVertexAttribPointer(prog->getAttribute("weights2"), 4, GL_FLOAT, GL_FALSE, stride, (const void *)( 8*sizeof(float)));
        glVertexAttribPointer(prog->getAttribute("weights3"), 4, GL_FLOAT, GL_FALSE, stride, (const void *)(12*sizeof(float)));


        GLSL::enableVertexAttribArray(prog->getAttribute("bones0"));
        GLSL::enableVertexAttribArray(prog->getAttribute("bones1"));
        GLSL::enableVertexAttribArray(prog->getAttribute("bones2"));
        GLSL::enableVertexAttribArray(prog->getAttribute("bones3"));
        glBindBuffer(GL_ARRAY_BUFFER, boneBufID);

        glVertexAttribPointer(prog->getAttribute("bones0"), 4, GL_FLOAT, GL_FALSE,stride, (const void *)( 0*sizeof(float)));
        glVertexAttribPointer(prog->getAttribute("bones1"), 4, GL_FLOAT, GL_FALSE, stride, (const void *)( 4*sizeof(float)));
        glVertexAttribPointer(prog->getAttribute("bones2"), 4, GL_FLOAT, GL_FALSE, stride, (const void *)( 8*sizeof(float)));
        glVertexAttribPointer(prog->getAttribute("bones3"), 4, GL_FLOAT, GL_FALSE, stride, (const void *)(12*sizeof(float)));


        glUniformMatrix4fv(prog->getUniform("animM"),18, GL_FALSE, animM[0].data());
        glUniformMatrix4fv(prog->getUniform("bindPose"),18, GL_FALSE, bindPose[0].data());

    }
    GLSL::enableVertexAttribArray(prog->getAttribute("numWeights"));
    glBindBuffer(GL_ARRAY_BUFFER, numWeightsBufID);
    glVertexAttribPointer(prog->getAttribute("numWeights"), 1, GL_INT, GL_FALSE, 0, 0);


    // Draw
    if (!wireframe){

        if(useAlpha){
          glEnable(GL_BLEND);
          glBlendFunc(GL_ONE, GL_ONE);

          glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

          glDisable(GL_BLEND);
        }
        else{
          glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
        }
    }
    else {
        glDrawElements(GL_LINES, nIndices, GL_UNSIGNED_INT, 0);
    }

    // Disable and unbind
    if(texBufID && h_tex >= 0) {
        GLSL::disableVertexAttribArray(h_tex);
    }
    if(norBufID && h_nor >= 0) {
        GLSL::disableVertexAttribArray(h_nor);
    }
    GLSL::disableVertexAttribArray(prog->getAttribute("numWeights"));
    GLSL::disableVertexAttribArray(h_pos);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    texture.unbind();
    //lightmap.unbind();
    //bumpmap.unbind();
    assert(glGetError() == GL_NO_ERROR);
}


Eigen::Matrix4f ShapeObj::getTotalMatforFrame(int joint, importsmd::keyframe kf, vector<int> parents) {
    std::vector<float> rots = kf.rotations;
    std::vector<float> pos = kf.positions;
    std::vector<Eigen::Matrix4f> mats;
    mats.clear();    

    do 
    {
        Eigen::Matrix3f mrot;
        Eigen::Vector3f r, p;

        r = Eigen::Vector3f(rots[joint*3+0], rots[joint*3+1], rots[joint*3+2]);
        p = Eigen::Vector3f(pos[joint*3+0], pos[joint*3+1], pos[joint*3+2]);
        mrot = Eigen::AngleAxisf(r(0), Eigen::Vector3f::UnitX()) *
               Eigen::AngleAxisf(r(1), Eigen::Vector3f::UnitY()) *
               Eigen::AngleAxisf(r(2), Eigen::Vector3f::UnitZ());

        Eigen::Matrix4f E = Eigen::Matrix4f::Identity();
        E.block<3,3>(0,0) = mrot;
        E.block<3,1>(0,3) = p;
        mats.push_back(E);

        joint = parents[joint];

    } while(joint != -1);

    Eigen::Matrix4f out = Eigen::Matrix4f::Identity();;

    for(int i = 0; i < mats.size(); i++) 
    {
        out = mats[i] * out;
    }

    return out;
}


std::vector<Eigen::Matrix4f> ShapeObj::getFrame(importsmd::keyframe kf) {
   
   std::vector<Eigen::Matrix4f> frame;
   frame.clear();
   
   int numJoints = smd_shapes[0].skeleton.numbones;
   
   for(int j = 0; j < numJoints; j++) {
      frame.push_back(getTotalMatforFrame(j, kf,  smd_shapes[0].skeleton.parent));
   }
   return frame;
}

void ShapeObj::AnimationToMatrix(int animationIndex) {
   int numFrames = smd_shapes[0].animations[animationIndex].numframes;
   std::vector<importsmd::keyframe> frames = smd_shapes[0].animations[animationIndex].frames;

   std::vector< std::vector<Eigen::Matrix4f>> out;

   for(int i = 0; i < numFrames; i++) {
      
      out.push_back(getFrame(frames[i]));
   }
   transformFrames.push_back(out);
   animM = transformFrames[transformFrames.size()-1][0];
}

int ShapeObj::getNumFrames(int animationIndex) {
    return smd_shapes[0].animations[animationIndex].numframes;
}

void ShapeObj::BindPoseToMatrix() {
   std::vector<importsmd::keyframe> frames = smd_shapes[0].animations[0].frames;   
   bindPose = getFrame(frames[0]);

   int numbones = smd_shapes[0].skeleton.numbones;
   for(int i = 0; i < numbones; i++) {
      Eigen::Matrix4f inv = bindPose[i].inverse();
      bindPose[i] = inv;
   }
   
}

void ShapeObj::loadSmdAnim(const std::string &filename) {
    if(animated == false) {
        int nVerts = smd_shapes[0].mesh.positions.size()/3;

        for(int i = 0; i < nVerts; i++) {
            for(int j = 0; j < 16; j++) {
                int numBonesAtVertex = smd_shapes[0].mesh.numbonesbinded[i];
                int startingPosofBoneList= smd_shapes[0].mesh.boneindices[i];
                if(j < numBonesAtVertex) {
                    int joint = smd_shapes[0].mesh.boneparents[startingPosofBoneList +j];


                    boneBuf.push_back(joint);
                    weightBuf.push_back(smd_shapes[0].mesh.boneweights[startingPosofBoneList +j]);

                }
                else {
                    boneBuf.push_back(0);
                    weightBuf.push_back(0);
                }
            }
        }    

        int numJoints = smd_shapes[0].skeleton.numbones;
        animM.clear();
        for(int i = 0; i < numJoints; i++) 
            animM.push_back(Eigen::Matrix4f::Identity());
       
        BindPoseToMatrix();
        animated = true;
    }
    
    importsmd::LoadSmdAnimation(smd_shapes, filename.c_str());
    AnimationToMatrix(transformFrames.size() + 1);
}

void ShapeObj::animate( int frame, int animation) {
   frame = frame% smd_shapes[0].animations[animation].numframes;
   animM.clear();
   
   animM = transformFrames[animation-1][frame];
}
