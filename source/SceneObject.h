#ifndef _SCENE_H_
#define _SCENE_H_

#include "GameObject.h"
#include "ShapeObj.h"
#include "Program.h"
#include "QuadTree.h"


class SceneObject { 
   
   public:
   SceneObject();

   void loadFile(const std::string &filename);
   void loadSmd(const std::string &filename);
   void init();

   void bindProg(Program *prog);
   void draw(MatrixStack *mv);
   void drawBounding(MatrixStack *mv, ShapeObj *sphere);
   void shadowDraw(MatrixStack *mv);
   void populateTree(QuadTree *tree);

   std::vector<GameObject *> props;
   std::vector<ShapeObj *> shapes;
};

#endif