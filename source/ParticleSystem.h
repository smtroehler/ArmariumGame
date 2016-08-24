#pragma  once
#ifndef __PARTICLE_SYS_H__
#define __PARTICLE_SYS_H__
//#define EIGEN_DONT_VECTORIZE
//#define EIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT
//#define EIGEN_DONT_ALIGN

#include "Particle.h"
//#include "../dependencies/Eigen/Dense"//unnecessary?
#include "GLSL.h"
#include "Program.h"
#include "MatrixStack.h"
#include "Texture.h"

struct point{
	float x;
	float y;
	float z;
};
class ParticleSystem{
public:
	ParticleSystem();
	virtual ~ParticleSystem();
	//Particle *particles;
	std::vector<Particle> particles;

	Program shader;
	int particlesPerSpawn;
	int numParticles;
	int liveParticles;
	bool dieing;
	void update(float dt);
	void draw(MatrixStack MV, MatrixStack P);
	void init();
	bool isDead();
	Particle spawnParticle();
	Eigen::Vector3f sourcePos;
	//bool particleSort(const Particle&,const Particle&);
protected:;
	GLfloat* particle_position_data;
	GLfloat* particle_life_data;
	GLuint vertex_buffer;
	GLuint position_buffer;
	GLuint life_buffer;
	GLuint tex_coord_buffer;
	Texture texture;
	void updateBuffers();
	int FindUnusedParticle();

};

#endif