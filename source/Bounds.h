#ifndef _BOUNDS_H_
#define _BOUNDS_H_

#include <iostream>

using namespace std;

class Bounds { 
   
   	public:
         Bounds();
   		Bounds(float x, float z, float w, float h);
   		virtual ~Bounds();
         void setVals(float x, float z, float w, float h);
         float getX(){ return centerX; }
         float getZ(){ return centerZ; }
         float getW(){ return width; }
         float getH(){ return height; }

      private:
         float centerX;
         float centerZ;
         float width;
         float height;
};

#endif