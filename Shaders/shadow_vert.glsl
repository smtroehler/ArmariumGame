#version 330 core

in vec4 vertPos;
uniform mat4 MV;
uniform mat4 P;

void main()
{
	gl_Position = P * MV * vertPos;
}