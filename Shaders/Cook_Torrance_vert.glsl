#version 330 core

in vec4 vertPos; // in object space
in vec3 vertNor; // in object space
in vec2 vertTex;

uniform mat4 P;
uniform mat4 MV;
uniform mat4 model;

out vec3 fragNor;
out vec3 fragPos;
out vec2 fragTex;

void main()
{
   gl_Position = P * MV * model * vertPos;
   fragNor = (MV * model * vec4(vertNor,0)).xyz;
   fragPos = (model * vertPos).xyz;
   
   fragTex = vertTex;
   
}
