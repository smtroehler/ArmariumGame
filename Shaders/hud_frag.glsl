#version 330 core

in vec2 fragTex;

uniform sampler2D texture0;
uniform sampler2D texture1;

out vec4 fragColor;

void main()
{

   //vec3 finalColor = vec3(0.0,0.0,0.0);
   
   
   vec4 tex;
   tex.rgb = texture(texture0, fragTex.st).rgb;
   vec3 mask =  texture(texture1, fragTex.st).rgb;
   tex.a = mask.r;

  fragColor = tex;

}
