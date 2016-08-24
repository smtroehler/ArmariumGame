#version 330 core

uniform vec3 ka;
//uniform vec3 kd;
//uniform vec3 ks;
uniform float s;

uniform vec3 lightPos; // in camera space
uniform sampler2D shadowMap;
in vec4 shadowClip;

//uniform float inten;

in vec3 fragNor;
in vec3 fragPos;
in vec2 fragTex;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 fragColor;


vec2 poissonDisk[] = vec2[](
    vec2( -0.94201624, -0.39906216 ),
    vec2( 0.94558609, -0.76890725 ),
    vec2( -0.094184101, -0.92938870 ),
    vec2( 0.34495938, 0.29387760 ),
    vec2( -0.91588581, 0.45771432 ),
    vec2( -0.81544232, -0.87912464 ),
    vec2( -0.38277543, 0.27676845 ),
    vec2( 0.97484398, 0.75648379 ),
    vec2( 0.44323325, -0.97511554 ),
    vec2( 0.53742981, -0.47373420 ),
    vec2( -0.26496911, -0.41893023 ),
    vec2( 0.79197514, 0.19090188 ),
    vec2( -0.24188840, 0.99706507 ),
    vec2( -0.81409955, 0.91437590 ),
    vec2( 0.19984126, 0.78641367 ),
    vec2( 0.14383161, -0.14100790 )
);



void main()
{
   vec3 n = normalize(fragNor);
   vec4 finalColor = vec4(0.0,0.0,0.0,0.0);
   
   
   vec3 tex = texture(texture0, fragTex.st).rgb;
   vec3 spec =  texture(texture1, fragTex.st).rgb;
   vec3 bump = texture(texture2, fragTex.st).rgb;
   vec3 normal = 2.0 * bump - vec3(1.0, 1.0, 1.0);
   
   
   vec3 l = normalize(lightPos - fragPos); 
   
   //vec3 color_d  = tex * max(dot(l, n), 0.0);
   
   vec3 color_d  = tex * max(dot(l, normal), 0.0);
   
   vec3 e = normalize(-fragPos);
   vec3 h = normalize(l + e); 
   vec3 color_s = spec * pow(max(dot(h, n), 0.0), s);
   
   
   //////////////////////////////////////////////////////
    // Shadowing
    //////////////////////////////////////////////////////
    
    // Convert from clip space to NDC
    vec4 shadowCoords = shadowClip;
    shadowCoords = shadowCoords / shadowCoords.w;
    // Go from [-1,1] to [0,1]
    shadowCoords.xyz = 0.5 * shadowCoords.xyz + 0.5;
    
    // Sample the shadow map N times
    float bias = 0.001;
    float blur = 0.001;
    float visibility = 1.0;
    if(shadowCoords.w > 0.0 &&
       shadowCoords.x > 0.0 && shadowCoords.x < 1.0 &&
       shadowCoords.y > 0.0 && shadowCoords.y < 1.0 &&
       shadowCoords.z > 0.0 && shadowCoords.z < 1.0) {
        for(int i = 0; i < 4; i++) {
            // Get the distance to light stored in the shadow map.
            // This value is in NDC between 0 and 1.
            // (Note: distToLight we computed for Blinn-Phong is in camera space.)
            float distToLightStored = texture(shadowMap, shadowCoords.xy + poissonDisk[i]*blur).z;
            if(distToLightStored < shadowCoords.z + bias) {
                visibility -= 0.25;
                //visibility = 1.0;
            }
        }
    }
    float texA = texture(texture0, fragTex.st).a;
    vec4 color = vec4(visibility * (color_d + color_s) + ka, texA);
    fragColor =  color;
}
