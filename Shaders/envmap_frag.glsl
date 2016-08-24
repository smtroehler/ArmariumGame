#version 330 core

//uniform sampler2D texture0;
uniform samplerCube texture0;

uniform float t;

in vec3 fragLight;

uniform mat4 P;
uniform mat4 M;
uniform mat4 V;

in vec3 fragNor;
in vec3 fragPos;
in vec2 fragTex;

in vec3 reflected;

out vec4 fragColor;

void main(void) {
    //use texture to read stuff
    float s = 100.0;
    vec3 UsColor = vec3(0.5, 0.7, 0.7);
    vec3 UdColor = vec3(0.5, 0.7, 0.7);
    vec3 UaColor = vec3(0.2, 0.2, 0.2);
     
    vec4 reflectedColor = texture(texture0, reflected);
    
    vec3 lightPos = vec3(0, 5.0, 5.0);
    
    vec3 l = normalize(fragLight - fragPos); 

    vec3 nPos = normalize(fragPos);
    vec3 h = l + nPos; 
    h = normalize(h);
    
    float num = max(dot(normalize(fragNor), l), 0.0);
    float spec = pow( max(dot(normalize(fragNor), h), 0.0), s); 
    vec4 shadeColor = vec4(spec * UsColor.x + num * UdColor.x + UaColor.x, 
                        spec * UsColor.y + num * UdColor.y + UaColor.y,
                        spec * UsColor.z + num * UdColor.z + UaColor.z,
                        1);
                        
    fragColor = shadeColor * reflectedColor;    
    //fragColor = vec4(reflected , 1);    
    //linearally interpolate between reflected and texture color with a lerp()
}

