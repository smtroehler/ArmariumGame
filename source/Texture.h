#ifndef __Texture__
#define __Texture__

#include "GLSL.h"
#include <cstdlib>


#include <string>

class Texture
{
public:
	Texture();
	virtual ~Texture();
	void setFilename(const std::string &f) { filename = f; }
	void init(bool alpha);
	void bind(GLint handle, GLint unit);
	void unbind();
	
private:
	std::string filename;
	int width;
	int height;
	GLuint tid;
	GLint unit;
	
};

#endif
