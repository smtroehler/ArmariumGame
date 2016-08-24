#version 330 core

in vec4 vertPos; // in object space
in vec2 vertTex;

out vec2 fragTex;

uniform mat4 P;
uniform mat4 MV;

void main()
{
   vec4 oas;
   oas = P * MV * vertPos;
   
   gl_Position = oas;
   fragTex = vertTex;
}
