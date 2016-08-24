#ifndef _IMPORTSMD_H
#define _IMPORTSMD_H


#include <string>
#include <vector>


namespace importsmd {
typedef struct
{
   std::vector<float>          positions;
   std::vector<float>          normals;
   std::vector<float>          texcoords;
   std::vector<unsigned int>   indices;

   std::vector<int>            boneindices; // starting index of parents/weights array
   std::vector<int>            numbonesbinded;
   std::vector<int>            boneparents; // per vertex its corresponding parents/weights will be boneindices to boneindices + numbonesbinded
   std::vector<float>          boneweights;
} mesh_smd;   

typedef struct 
{
   std::vector<std::string>    bone_name;
   std::vector<int>            parent;
   int                         numbones;
} skel_smd;


typedef struct {
   std::vector<float>          positions;
   std::vector<float>          rotations;
} keyframe;


typedef struct 
{
   std::vector<keyframe>       frames;
   std::string                 animName; 
   int                         numframes;
} anim;

typedef struct
{
    std::string                name;
    mesh_smd                   mesh;
    skel_smd                   skeleton;
    std::vector<anim>          animations;
    int                        numAnimations;
} shape_smd;




   void LoadSmd(
    std::vector<shape_smd>& shapes, 
    const char* filename);

   void LoadSmdAnimation(
    std::vector<shape_smd>& shapes, 
    const char* filename);


}
#endif