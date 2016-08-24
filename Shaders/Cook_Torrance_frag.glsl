#version 330 core

in vec3 fragNor;
in vec3 fragPos;
in vec2 fragTex;

uniform sampler2D texture0;
//uniform sampler2D texture1;
uniform vec3 ambI;

struct Light {
    vec3 lightPos;
    vec3 lumI;
};

struct material {
    vec3 ka;
    float m;
    vec3 fresnel0;
};

uniform Light lights[1];
uniform material mat;
out vec4 color;

float MicrofacetDist(float angle)
{
    return 100 * exp(-(angle * angle)/ (mat.m * mat.m));
    //return exp(-pow((tan(angle)/mat.m), 2) / (4.0 * mat.m * mat.m * pow(cos(angle), 4)));
}

void main(){
   vec3 ks = vec3(0.5, 0.5, 0.5);//texture(texture1, fragTex.st).rgb;
   vec3 kd = texture(texture0, fragTex.st).rgb;
   vec3 n = normalize(fragNor);
   
   vec3 l = normalize(lights[0].lightPos - fragPos); 
   
   vec3 v = normalize(0.0 - fragPos);
   color = vec4(abs(-fragPos), 1.0);
   vec3 h = normalize(l + v); 

   float NdotH = dot(n, h);
   float NdotL = dot(n, l);
   float VdotH = dot(v, h);
   float NdotV = dot(n, v);

   float g = min(min(1.0, (2 * NdotH * NdotV / VdotH)), (2 * NdotH * NdotL / VdotH));
   float d = MicrofacetDist(acos(NdotH));
   vec3 fresnel = mat.fresnel0 + (1.0 - mat.fresnel0) * pow(1 - dot(h, v), 5.0);

   //color = vec4(ambI * mat.ka + (lights[0].lumI * ((1 - mat.ka - ks) * kd * dot(l, n) + ks * ((fresnel * d * g) / (3.14 * dot(v, n))))), 1.0);
}