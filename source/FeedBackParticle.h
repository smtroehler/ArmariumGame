#ifndef _FEEDBACK_PARTICLES_H
#define _FEEDBACK_PARTICLES_H
#include "GLSL.h"

class FeedBackParticle
{
    public:
    FeedBackParticle();
    void init();
    void bindProg(Program* prog);
    void update();
private:
	GLuint BufferA, BufferB, Feedback;
	GLuint progHandle, vertexShader;
	const int particleCount = 10000;
	Particle seed_particles[particleCount] = {...};
};

#endif