#ifndef __CubeMap__
#define __CubeMap__

#include "GLSL.h"
#include "Texture.h"

#include "../dependencies/Eigen/Dense"
#include "../dependencies/Eigen/Geometry"
#include "../dependencies/Eigen/LU"


#include <string>
#include <vector>


class CubeMap
{
public:
	CubeMap();
	virtual ~CubeMap();
    void setFaces(std::vector<std::string> &f) { faces = f; }

    MatrixStack selectFace(Eigen::Vector3f center, int i);
    void apply(Program prog);
    
	void init();
	void bind(GLint handle, GLint unit);
	void unbind();
	
private:
	std::vector<std::string> faces;
    
	int width;
	int height;
	
    GLuint tid;
    GLuint framebuffer;
    GLuint depthbuffer;


    GLint unit;
};

#endif
