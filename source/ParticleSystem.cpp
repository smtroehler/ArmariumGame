#include "ParticleSystem.h"
#define EIGEN_DONT_VECTORIZE
#define EIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT
#define EIGEN_DONT_ALIGN
#include "stdio.h"

using namespace std;
ParticleSystem::ParticleSystem()
{
    numParticles = 500;
    particle_position_data = new GLfloat[numParticles * 4];
    particle_life_data = new GLfloat[numParticles * 4];
    liveParticles = 0;
    //particles = new Particle[numParticles];
    particlesPerSpawn = 30;
    shader.setShaderNames("Shaders/particle_vert.glsl", "Shaders/particle_frag.glsl");
    texture.setFilename("Assets/Textures/explosion19.png");
}

ParticleSystem::~ParticleSystem(){};

Particle ParticleSystem::spawnParticle()
{
    Particle newPart;
    //get position in particle array
    int index = FindUnusedParticle();
    newPart.setLocation(sourcePos);
    //set position
    //set life
    newPart.life = 0.0f;
    //set velocity
    newPart.setVel(Eigen::Vector3f((rand() % 2000 - 1000.0f) / 1500.0f, 0.8f, (rand() % 2000 - 1000.0f) / 1500.0f));
    //set color
    newPart.r = (rand() % 256);
    newPart.g = (rand() % 256);
    newPart.b = (rand() % 256);
    newPart.a = (rand() % 256);
    return newPart;
}

void ParticleSystem::init()
{

    dieing = false;

    static const GLfloat vert_buf_data[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f
    };

    static const GLfloat vert_tex_coords[] = {
        0.0f, 0.2f,
        0.2f, 0.2f,
        0.0f, 0.0f,
        0.2f, 0.0f
    };

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert_buf_data), vert_buf_data, GL_STATIC_DRAW);

    //A buffer to contain the positions of each particle
    //GL_STREAM_DRAW because this data will be updated
    glGenBuffers(1, &position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
    glBufferData(GL_ARRAY_BUFFER, numParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

    //Temporary Color buffer for testing purposes
    glGenBuffers(1, &life_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, life_buffer);
    glBufferData(GL_ARRAY_BUFFER, numParticles * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

    glGenBuffers(1, &tex_coord_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, tex_coord_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert_tex_coords), vert_tex_coords, GL_STATIC_DRAW);
    assert(glGetError() == GL_NO_ERROR);
    texture.init(true);
}

void ParticleSystem::updateBuffers()
{
    glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
    glBufferData(GL_ARRAY_BUFFER, numParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, liveParticles * sizeof(GLfloat) * 4, particle_position_data);

    glBindBuffer(GL_ARRAY_BUFFER, life_buffer);
    glBufferData(GL_ARRAY_BUFFER, numParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, liveParticles * sizeof(GLfloat), particle_life_data);
}

int ParticleSystem::FindUnusedParticle()
{
    static int position = 0;
    return position++ % numParticles;
}

bool particleSort(const Particle& one, const Particle& two)
{
    return one.distToCamera > two.distToCamera;
}

void ParticleSystem::update(float dt)
{
    //add new particles
    int newparticles = min((int)(dt * particlesPerSpawn * 10000), 10);
    for (int i = 0; i < newparticles && liveParticles < numParticles && !dieing; i++)
    {
        particles.push_back(spawnParticle());
        if (liveParticles < numParticles) liveParticles++;
    }

    //simulate particles
    for (int ndx = 0; ndx < liveParticles; ndx++)
    {
        Particle& current = particles[ndx];
        current.life += dt * 20;
        if (current.life < 10.0f)
        {
            current.updateLocation(Eigen::Vector3f(current.getVel().x() * dt, current.getVel().y() * dt, current.getVel().z() * dt));

            particle_position_data[4 * ndx + 0] = current.getCenter()(0);
            particle_position_data[4 * ndx + 1] = current.getCenter()(1);
            particle_position_data[4 * ndx + 2] = current.getCenter()(2);
            particle_position_data[4 * ndx + 3] = 0.75f;


            //instead of this i will bed calculating textcoords based on life
            particle_life_data[ndx] = current.life;
        }
        else if (!dieing)
        {
            current.setLocation(sourcePos);
            current.life = 0.0f;
            particle_life_data[ndx] = current.life;
        }
        else {
            particles.erase(particles.begin() + ndx);
            ndx--;
            liveParticles--;
        }
        current.distToCamera = (current.getCenter() - Eigen::Vector3f(0.0, 1.5, 6.0)).norm();
    }
    std::sort(particles.begin(), particles.end(), particleSort);
}

void ParticleSystem::draw(MatrixStack MV, MatrixStack P)
{
    shader.bind();
    updateBuffers();
    // 1rst attribute buffer : vertices

    glEnableVertexAttribArray(shader.getAttribute("squareVertices"));
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexAttribPointer(
        shader.getAttribute("squareVertices"), // attribute. No particular reason for 0, but must match the layout in the shader.
        3, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0, // stride
        (void*)0 // array buffer offset
        );

    // 2nd attribute buffer : positions of particles' centers
    glEnableVertexAttribArray(shader.getAttribute("xyzs"));
    glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
    glVertexAttribPointer(
        shader.getAttribute("xyzs"), // attribute. No particular reason for 1, but must match the layout in the shader.
        4, // size : x + y + z + size => 4
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0, // stride
        (void*)0 // array buffer offset
        );

    // 3rd attribute buffer : particles' colors
    glEnableVertexAttribArray(shader.getAttribute("partLife"));
    glBindBuffer(GL_ARRAY_BUFFER, life_buffer);
    glVertexAttribPointer(
        shader.getAttribute("partLife"), // attribute. No particular reason for 1, but must match the layout in the shader.
        1, // size : r + g + b + a => 4
        GL_FLOAT, // type
        GL_FALSE, // normalized? *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
        0, // stride
        (void*)0 // array buffer offset
        );

    glEnableVertexAttribArray(shader.getAttribute("TexCoord"));
    glBindBuffer(GL_ARRAY_BUFFER, tex_coord_buffer);
    glVertexAttribPointer(
        shader.getAttribute("TexCoord"), // attribute. No particular reason for 1, but must match the layout in the shader.
        2, // size : x, y
        GL_FLOAT, // type
        GL_FALSE, // normalized? *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
        0, // stride
        (void*)0 // array buffer offset
        );


    glVertexAttribDivisor(shader.getAttribute("squareVertices"), 0); // particles vertices : always reuse the same 4 vertices -> 0
    glVertexAttribDivisor(shader.getAttribute("xyzs"), 1); // positions : one per quad (its center) -> 1
    glVertexAttribDivisor(shader.getAttribute("partLife"), 1); // color : one per quad -> 1
    glVertexAttribDivisor(shader.getAttribute("TexCoord"), 0);

    glUniformMatrix4fv(shader.getUniform("VP"), 1, GL_FALSE, ((Eigen::Matrix<float, 4, 4>)(P.topMatrix() * MV.topMatrix())).data());
    glUniform3f(shader.getUniform("CameraRight_worldspace"), MV.topMatrix()(0, 0), MV.topMatrix()(1, 0), MV.topMatrix()(2, 0));
    glUniform3f(shader.getUniform("CameraUp_worldspace"), 0.0f, 1.0f, 0.0f);//MV.topMatrix()(0, 1), MV.topMatrix()(1, 1), MV.topMatrix()(2, 1));
    //BIND TEXTURE
    texture.bind(shader.getUniform("tex"), 0);
    //printf("Live parts %d\n", liveParticles);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, liveParticles);
    texture.unbind();
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(shader.getAttribute("squareVertices"), 0); // particles vertices : always reuse the same 4 vertices -> 0
    glVertexAttribDivisor(shader.getAttribute("xyzs"), 0); // positions : one per quad (its center) -> 1
    glVertexAttribDivisor(shader.getAttribute("partLife"), 0); // color : one per quad -> 1
    glVertexAttribDivisor(shader.getAttribute("TexCoord"), 0);
    shader.unbind();
}

bool ParticleSystem::isDead()
{
    return particles.size() == 0;
}
