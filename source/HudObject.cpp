#include "HudObject.h"
#include <iostream>

extern int width;
extern int height;

HudObject::HudObject() :
   posBufID(0),
   texBufID(0),
   indBufID(0)
{
   texture.setFilename("Assets/Textures/missing_texture.jpg"); 
   blendMap.setFilename("Assets/Textures/missing_lightmap.jpg"); 
}



void HudObject::loadFile(const std::string &meshName) {
   std::vector<tinyobj::material_t> objMaterials;
   std::string err = tinyobj::LoadObj(shapes, objMaterials, meshName.c_str());
   if(!err.empty()) {
      std::cerr << err << std::endl;
   }
}


void HudObject::init() {


   texture.init(false);
   blendMap.init(false);

   const std::vector<float> &posBuf = shapes[0].mesh.positions;
   glGenBuffers(1, &posBufID);
   glBindBuffer(GL_ARRAY_BUFFER, posBufID);
   glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);

   const std::vector<float> &norBuf = shapes[0].mesh.normals;
   if(!norBuf.empty()) {
      glGenBuffers(1, &norBufID);
      glBindBuffer(GL_ARRAY_BUFFER, norBufID);
      glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_STATIC_DRAW);
   } else {
      norBufID = 0;
   }
   
   // Send the texture coordinates array (if it exists) to the GPU
   const std::vector<float> &texBuf = shapes[0].mesh.texcoords;
   if(!texBuf.empty()) {
      glGenBuffers(1, &texBufID);
      glBindBuffer(GL_ARRAY_BUFFER, texBufID);
      glBufferData(GL_ARRAY_BUFFER, texBuf.size()*sizeof(float), &texBuf[0], GL_STATIC_DRAW);
   } else {
      texBufID = 0;
   }

   // Send the index array to the GPU
   const std::vector<unsigned int> &indBuf = shapes[0].mesh.indices;
   glGenBuffers(1, &indBufID);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufID);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, indBuf.size()*sizeof(unsigned int), &indBuf[0], GL_STATIC_DRAW);
   
   // Unbind the arrays
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   
   assert(glGetError() == GL_NO_ERROR);
}


// Used to rescale the UV so texture doesn't get condensed and fuzzy
void HudObject::rescale() {
   if(scale(0) != scale(1)) {
      std::vector<float> texBuf = shapes[0].mesh.texcoords;
      float scaleX = scale(0)  / scale(1);
      float scaleY = scale(1)  / scale(0);
      for(int i = 0; i < texBuf.size()/2; i++) {
         if (scaleX < 1)
            texBuf[2*i+0] *= scaleX;
         if (scaleY < 1)
            texBuf[2*i+1] *= scaleY;
      
      }
      
      if(!texBuf.empty()) {
         glGenBuffers(1, &texBufID);
         glBindBuffer(GL_ARRAY_BUFFER, texBufID);
         glBufferData(GL_ARRAY_BUFFER, texBuf.size()*sizeof(float), &texBuf[0], GL_STATIC_DRAW);
      } else {
         texBufID = 0;
      }
   }
}

void HudObject::mirror() {
   
   std::vector<float> &texBuf = shapes[0].mesh.texcoords;
   
   for(int i = 0; i < texBuf.size()/2; i++) {
      texBuf[2*i+0] = 1 - texBuf[2*i+0];
   }
   
   if(!texBuf.empty()) {
      glGenBuffers(1, &texBufID);
      glBindBuffer(GL_ARRAY_BUFFER, texBufID);
      glBufferData(GL_ARRAY_BUFFER, texBuf.size()*sizeof(float), &texBuf[0], GL_STATIC_DRAW);
   } else {
      texBufID = 0;
   }
}


void HudObject::draw(MatrixStack *P, MatrixStack *MV) {

   glDisable(GL_DEPTH_TEST);

   MV->pushMatrix();
   P->pushMatrix();

   MV->loadIdentity();
   P->loadIdentity();

   P->ortho(0, width, 0, height, -100, 100); //warning conversion from int to float
   //MV->scale(Eigen::Vector3f(width / size_w, height / size_h , 1.0 ));
   MV->multMatrix(P->topMatrix());
   
   Eigen::Vector3f p;
   p(0) = (width / size_w) * pos(0);
   p(1) = (height / size_h) * pos(1);
   p(2) = pos(2);
   MV->translate(p);

   
   MV->scale(Eigen::Vector3f(scale(0) * width / size_w, scale(1) * height / size_h , scale(2)  ));
   MV->rotate(rot(0), Eigen::Vector3f(1, 0, 0));
   P->loadIdentity();

   prog->bind();

   texture.bind(prog->getUniform("texture0"), 0);
   blendMap.bind(prog->getUniform("texture1"), 1);

   glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, P->topMatrix().data());
   glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, MV->topMatrix().data());

   




   int h_pos = prog->getAttribute("vertPos");
   
   GLSL::enableVertexAttribArray(h_pos);
   glBindBuffer(GL_ARRAY_BUFFER, posBufID);
   glVertexAttribPointer(h_pos, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
   int h_tex = prog->getAttribute("vertTex");
   if(texBufID && h_tex >= 0) {
      GLSL::enableVertexAttribArray(h_tex);
      glBindBuffer(GL_ARRAY_BUFFER, texBufID);
      glVertexAttribPointer(h_tex, 2, GL_FLOAT, GL_FALSE, 0, 0);
   }
   // Bind index array for drawing
   int nIndices = (int)shapes[0].mesh.indices.size();
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufID);
   
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   // Draw

   glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
   
   // Disable and unbind
   if(texBufID && h_tex >= 0) {
      GLSL::disableVertexAttribArray(h_tex);
   }
   GLSL::disableVertexAttribArray(h_pos);
   glDisable(GL_BLEND);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   texture.unbind();
   blendMap.unbind();
   prog->unbind();
   MV->popMatrix();
   P->popMatrix();
   glEnable(GL_DEPTH_TEST);
}