#include <stdio.h>
//#include <stdlib.h>
#include <iostream>

#include "stb_image.h"
#include "Camera.h"
#include "CubeMap.h"

CubeMap::CubeMap() :
	tid(0)
{
	
}

CubeMap::~CubeMap()
{
	
}

void CubeMap::init()
{
	// Load texture
	int w, h, ncomps;
	unsigned char *data = NULL;

    glGenTextures(1, &tid);
    glActiveTexture(GL_TEXTURE0);	
    glBindTexture(GL_TEXTURE_CUBE_MAP, tid);

    for (GLuint i = 0; i < faces.size(); i++)
    {
        //Read and set the faces.
        data = stbi_load(faces[i].c_str(), &w, &h, &ncomps, STBI_rgb);

        if (!data) {
            std::cerr << faces[i] << " not found" << std::endl;
        }
        if ((w & (w - 1)) != 0 || (h & (h - 1)) != 0) {
            std::cerr << faces[i] << " must be a power of 2 w:" << w << " h: " << h << std::endl;
        }
        width = w;
        height = h;

        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
            GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);


    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    //glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    stbi_image_free(data);

    //VBO.
    // create the fbo
    //glGenFramebuffers(1, &framebuffer);
    //glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // create the uniform depth buffer
    //glGenRenderbuffers(1, &depthbuffer);
    //glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer);
    //glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    //glBindRenderbuffer(GL_RENDERBUFFER, 0);
    //GLenum drawBufs[] = { GL_COLOR_ATTACHMENT0 };
    // attach it
    //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, framebuffer);
    // attach only the +X cubemap texture (for completeness)
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, tid, 0);
    //glDrawBuffers(1, drawBufs);

    // disable
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    assert(glGetError() == GL_NO_ERROR);
}

//should return V matrix.
MatrixStack CubeMap::selectFace(Eigen::Vector3f center, int i){
    MatrixStack V;
    // attach new texture and renderbuffer to fbo
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    assert(glGetError() == GL_NO_ERROR);
    // clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //printf("%d\n", i);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, tid, 0);
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    // play around with the near and far values
    //gluPerspective(90, 1, 1, 1000);
    glViewport(0, 0, 512, 512);

    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
    

    // setup lookat depending on current face
    switch (i)
    {
    case 0: //POSITIVE_X
        V.lookAt(center, Eigen::Vector3f(1, 0, 0), Eigen::Vector3f(0, 1, 0));
        break;

    case 1: //NEGATIVE_X
        V.lookAt(center, Eigen::Vector3f(-1, 0, 0), Eigen::Vector3f(0, 1, 0));
        break;

    case 2: //POSITIVE_Y
        V.lookAt(center, Eigen::Vector3f(0, 10, 0), Eigen::Vector3f(1, 0, 0));
        break;

    case 3: //NEGATIVE_Y
        V.lookAt(center, Eigen::Vector3f(0, -10, 0), Eigen::Vector3f(1, 0, 0));
        break;

    case 4: //POSITIVE_Z
        V.lookAt(center, Eigen::Vector3f(0, 0, 10), Eigen::Vector3f(0, 1, 0));
        break;

    case 5: //NEGATIVE_Z
        V.lookAt(center, Eigen::Vector3f(0, 0, -10), Eigen::Vector3f(0, 1, 0));
        break;

    default:
        break;
    };

    assert(glGetError() == GL_NO_ERROR);
    return V;
}


void CubeMap::bind(GLint handle, GLint unit)
{
	this->unit = unit;
	glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, tid);
	glUniform1i(handle, unit);
}

void CubeMap::unbind()
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, 0);
}
