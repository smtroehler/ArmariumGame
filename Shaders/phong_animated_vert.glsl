#version 330 core

in vec4 vertPos; // in object space
in vec3 vertNor; // in object space
in vec2 vertTex;

uniform mat4 P;
uniform mat4 MV;
uniform mat4 lightMVP;

out vec3 fragNor;
out vec3 fragPos;
out vec2 fragTex;
out vec4 shadowClip;


in vec4 weights0;
in vec4 weights1;
in vec4 weights2;
in vec4 weights3;

in vec4 bones0;
in vec4 bones1;
in vec4 bones2;
in vec4 bones3;

in float numWeights;
uniform mat4 animM[18];
uniform mat4 bindPose[18];


void main()
{
   
   
   vec4 total1 = vec4(0.0,0.0,0.0,0.0);
   vec4 total2 = vec4(0.0,0.0,0.0,0.0);
   
   if(int(numWeights) == 0) {
       total1 = vertPos;
       total2 = vec4(vertNor.xyz, 0.0);
   }

   for(int i = 0; i < int(numWeights); i++) {
      int joint = 0;
      float weight = 0.0;
      if(i < 4) {
         joint = int(bones0[i]);
         weight = weights0[i];
      }
      else if(i < 8) {
         joint = int(bones1[i-4]);
         weight = weights1[i-4];
      }
      else if(i < 12) {
         joint = int(bones2[i-8]);
         weight = weights2[i-8];
      }
      else if(i < 16) {
         joint = int(bones3[i-12]);
         weight = weights3[i-12];
      }
      total1 += weight * animM[joint] * bindPose[joint] * vertPos;
      total2 += weight * animM[joint] * bindPose[joint] * vec4(vertNor.xyz, 0.0);
   }


   vec4 verTot = MV * total1;
   
   gl_Position = P * verTot;
   fragNor = (MV * total2).xyz;
   fragPos = (verTot).xyz;
   
   fragTex = vertTex;
   
   // vertex position in light clip place
   shadowClip = lightMVP * total1;
}
