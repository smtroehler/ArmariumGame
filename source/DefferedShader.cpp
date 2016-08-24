#include "DefferedShader.h"
DefferedShader::DefferedShader(){};
DefferedShader::~DefferedShader(){};

void DefferedShader::init(int w, int h)
{

    width = w;
    height = h;
	prog.setShaderNames("Shaders/PassThru.glsl", "Shaders/PassThru_frag.glsl");
    prog.init();
    prog.addAttribute("vertexPosition_modelspace");
    prog.addUniform("renderedTexture");
    prog.addUniform("norTexture");
    prog.addUniform("posTexture");
    prog.addUniform("mode");
    prog.addUniform("cameraPos");
    prog.addUniform("numLights");

    prog.addUniform("lightMVP");
    prog.addUniform("shadowMap");

    glGenFramebuffers(1, &colorBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, colorBuffer);


    glGenTextures(1, &renderedTexture);
    glBindTexture(GL_TEXTURE_2D, renderedTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

    glGenTextures(1, &norTexture);
    glBindTexture(GL_TEXTURE_2D, norTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, norTexture, 0);

    glGenTextures(1, &posTexture);
    glBindTexture(GL_TEXTURE_2D, posTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, posTexture, 0);

    GLuint depthrenderbuffer;
    glGenRenderbuffers(1, &depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

    GLenum DrawBuffers[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, DrawBuffers); // "2" is the size of DrawBuffers
    glDrawBuffers(3, DrawBuffers); 

    // The fullscreen quad's FBO
    glGenVertexArrays(1, &quad_VertexArrayID);
    glBindVertexArray(quad_VertexArrayID);

    static const GLfloat g_quad_vertex_buffer_data[] = {
       -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f,0.0f,
        -1.0f,  1.0f,0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,
    };

    glGenBuffers(1, &quad_vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
    assert(glGetError() == GL_NO_ERROR);
}

void DefferedShader::start()
{
    glBindFramebuffer(GL_FRAMEBUFFER, colorBuffer);
    //glViewport(0,0,width,height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, renderedTexture);

    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, norTexture);

    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, posTexture);
    assert(glGetError() == GL_NO_ERROR);
}

void DefferedShader::bind()
{
    prog.bind();
}

void DefferedShader::setLightData(std::vector<Light *> lights){
    int ndx;
    std::string base;
    for(ndx = 0; ndx < lights.size(); ndx++)
    {   
        base = "lights[" + std::to_string(ndx) + "]";
        GLuint loc = glGetUniformLocation(prog.getPid(), (base + std::string(".lightPos")).c_str());
        glUniform3fv(loc,1, lights[ndx]->getPosition().data());
        loc = glGetUniformLocation(prog.getPid(), (base + std::string(".lumI")).c_str());
        glUniform3fv(loc,1, lights[ndx]->getLuminance().data());
        loc = glGetUniformLocation(prog.getPid(), (base + std::string(".ambI")).c_str());
        glUniform3fv(loc,1, lights[ndx]->getAmbience().data());
        loc = glGetUniformLocation(prog.getPid(), (base + std::string(".cosntAtten")).c_str());
        glUniform1f(loc, lights[ndx]->getAtten().x());
        loc = glGetUniformLocation(prog.getPid(), (base + std::string(".linAtten")).c_str());
        glUniform1f(loc,lights[ndx]->getAtten().y());
        loc = glGetUniformLocation(prog.getPid(), (base + std::string(".quadAtten")).c_str());
        glUniform1f(loc, lights[ndx]->getAtten().z());
    }
    assert(glGetError() == GL_NO_ERROR);
}

void DefferedShader::setShadowData(GLint shadowMap, Eigen::Matrix4f lightMVP, GLint shadowmap){
    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    glUniform1i(prog.getUniform("shadowMap"), shadowmap);
    glUniformMatrix4fv(prog.getUniform("lightMVP"), 1, GL_FALSE, lightMVP.data());
}

void DefferedShader::render(int mode, Eigen::Vector3f camera, int numLights){
	//prog.bind();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0,0,width,height);

    // Set our "renderedTexture" sampler to user Texture Unit 0
    glUniform1i(prog.getUniform("renderedTexture"), 4);
    glUniform1i(prog.getUniform("norTexture"), 5);
    glUniform1i(prog.getUniform("posTexture"), 6);
    glUniform1i(prog.getUniform("mode"), mode);
    glUniform1i(prog.getUniform("numLights"), numLights);
    glUniform3fv(prog.getUniform("cameraPos"),1, camera.data());

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(prog.getAttribute("vertexPosition_modelspace"));
    glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
    glVertexAttribPointer(
        prog.getAttribute("vertexPosition_modelspace"), // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    // Draw the triangles !
    glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles

    glDisableVertexAttribArray(prog.getAttribute("vertexPosition_modelspace"));
    prog.unbind();

}