#version 330 core

out vec4 shadowClip; //Shadow stuff!
uniform mat4 lightMVP;

in vec3 vertexPosition_modelspace;
uniform sampler2D posTexture;
// Output data ; will be interpolated for each fragment.
out vec2 UV;

void main(){
	gl_Position =  vec4(vertexPosition_modelspace,1);
	UV = (vertexPosition_modelspace.xy+vec2(1,1))/2.0;
    
    // vertex position in light clip place
   shadowClip = lightMVP * vec4(vertexPosition_modelspace,1);
}

