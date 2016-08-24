#version 330 core

in vec3 fragNor;
in vec3 fragPos;
in vec2 fragTex;

uniform sampler2D texture0;

layout (location = 0) out vec4 color;
layout (location = 1) out vec3 norColor;
layout (location = 2) out vec3 position;
//layout (location = 3) out vec4 specular; //material ks and m
//layout (location = 5) out vec3 fresnel; //material fresnel

void main()
{
	vec3 tex = texture(texture0, fragTex.st).rgb;
	color = vec4(tex, 1.0);
	norColor = normalize(fragNor);
    position = fragPos;     

}