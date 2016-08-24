#include "Bounds.h"
using namespace std;

Bounds::Bounds(){}

Bounds::Bounds(float x, float z, float w, float h){
	centerX = x;
	centerZ = z;
	width = w;
	height = h;
}

Bounds::~Bounds(){}

void Bounds::setVals(float x, float z, float w, float h){
	centerX = x;
	centerZ = z;
	width = w;
	height = h;
}