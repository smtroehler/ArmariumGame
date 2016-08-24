#include "FeedBackParticle.h"

FeedBackParticle::FeedBackParticle(){};

FeedBackParticle::init()
{

	glGenBuffers(1, &BufferA);
	glBindBuffer(GL_ARRAY_BUFFER, BufferA);
	glBufferData(GL_ARRAY_BUFFER, sizeof(seed_particles), &seed_particles[0], GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
 
	// Create VBO for output on even-numbered frames and input on odd-numbered frames:
	glGenBuffers(1, &BufferB);
	glBindBuffer(GL_ARRAY_BUFFER, BufferB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(seed_particles), 0, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

    Feedback = glGenTransformFeedbacks(1, &Feedback);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, Feedback);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, BufferA);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

	programHandle = glCreateProgram();
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, "FeedbackParticle.glsl", 0);
	glCompileShader(vertexShader);
	glAttachShader(programHandle, vertexShader);

	const char* varyings[3] = { "vPosition", "vBirthTime", "vVelocity" };
	glTransformFeedbackVaryings(programHandle, 3, varyings, GL_INTERLEAVED_ATTRIBS);
	glLinkProgram(programHandle);
}

FeedbackParticle::update()
{
    glEnable(GL_RASTERIZER_DISCARD);
    glBindBuffer(GL_ARRAY_BUFFER, BufferA);
    //glVertexAttribPointer...
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, Feedback);
    glBeginTransformFeedback(GL_POINTS);
    glDrawArrays(GL_POINTS, 0, ParticleCount);
    glEndTransformFeedback();
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
    swap(BufferA, BufferB);
}

FeedbackParticle::draw()
{
	glDisable(GL_RASTERIZER_DISCARD);
	glBindBuffer(GL_ARRAY_BUFFER, BUFFERA);
	//glVertexAttribPointer(...);
	glDrawTransformFeedback(GL_POINTS, Feedback);
	// Set up the advection shader:
	glUseProgram(ParticleAdvectProgram);
	glUniform1f(ParticleAdvectProgram, timeLoc, currentTime);

	// Specify the source buffer:
	glEnable(GL_RASTERIZER_DISCARD);
	glBindBuffer(GL_ARRAY_BUFFER, ParticleBufferA);
	glEnableVertexAttribArray(SlotPosition);
	glEnableVertexAttribArray(SlotBirthTime);
}