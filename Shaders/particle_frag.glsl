#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec2 fragTexCoord;
uniform sampler2D tex;

out vec4 fragColor;

void main(){
	// Output color = color of the texture at the specified UV
	vec4 color = texture(tex, fragTexCoord.st).rgba;
	if (color.a < 0.2)
	   discard;
	else
		fragColor = color;

}