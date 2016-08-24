#version 330 core

in vec2 aTexCoord;

uniform mat4 P;
uniform mat4 M;
uniform mat4 V;
uniform vec3 camLoc;

in vec3 lightWorld;
out vec3 fragLight;

in vec4 vertPos; // in object space
in vec3 vertNor; // in object space
in vec2 vertTex;

out vec3 fragNor;
out vec3 fragPos;
out vec2 fragTex;

out vec3 reflected;

uniform sampler2D uTexUnit;

void main() {

  fragTex = aTexCoord;

  vec4 temp = P * V * M * vertPos;
  fragPos =  temp.xyz;
  gl_Position = temp;
  
  
  vec4 light = V * vec4(lightWorld, 1);
  fragLight = light.xyz;
  
  fragNor = vertNor;
  fragTex = vertTex;
  
  //actually mult by M ONLY
  //invident is opposite of view vector used in phong
  vec4 posWorld = M * vertPos;
  vec3 incident = camLoc - posWorld.xyz;
  vec4 norWorld = M * vec4(vertNor, 1);

  
  float ratio = 1.00 / 1.52;
  vec3 i = normalize(posWorld.xyz - camLoc);
  reflected = reflect(incident, norWorld.xyz);
  //reflected = refract(i, normalize(norWorld.xyz), ratio);    
}
