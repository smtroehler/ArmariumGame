#version 330 core

in vec2 UV;

uniform vec3 cameraPos;
uniform int mode;

uniform sampler2D renderedTexture;
uniform sampler2D norTexture;
uniform sampler2D posTexture;

out vec4 color;

struct Light {
    vec3 lightPos;
    vec3 ambI;
    vec3 lumI;
    float constAtten;
    float linAtten;
    float quadAtten;
};

uniform Light lights[40];
uniform int numLights;
float m = 0.005;


//shadow stuff
uniform sampler2D shadowMap;
in vec4 shadowClip;

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

float MicrofacetDist(float angle)
{
    return 1 * exp(-(angle * angle)/ (m * m));
    //return max(0.0, exp(-pow((tan(angle)/m), 2)) / (4.0 * m * m * pow(cos(angle), 4)));
}


void main(){
    vec3 tex = texture( renderedTexture, UV).xyz ;
	vec3 normal = texture(norTexture, UV).xyz;
	vec3 fragPos = texture(posTexture, UV).xyz;
    vec3 ka = vec3(0.1,0.1,0.1);
    vec3 fresnel0 = vec3(0.1, 0.1, 0.1);
    color = vec4(0.0, 0.0, 0.0,0.0);
    if(mode == 1) {
        color = vec4(tex, 1.0);
    }
	else if(mode == 2) {
        color = vec4(normal, 1.0);
    }
    else if(mode == 3) {
        color = vec4(fragPos, 1.0);
    }
    else { //Do everything.
        vec3 ks = vec3(0.5, 0.5, 0.5);//texture(texture1, fragTex.st).rgb;
        vec3 kd = tex;
        vec3 n = normalize(normal);
        int light;
        float distance, attenuation, NdotH, NdotL, VdotH, NdotV, g, d;
        vec3 lightDir, l, v, h, fresnel;
   
        //Apply lights
        color = vec4(vec3(0.01, 0.01, 0.01) * ka, 1.0);
        for (light = 0; light < numLights; light++) {
            lightDir = lights[light].lightPos - fragPos;
            l = normalize(lightDir); 

            distance = abs(length(lightDir));
            attenuation = min(2.0, 1 / (lights[light].constAtten + distance * lights[light].linAtten + distance * distance * lights[light].quadAtten));
   
            v = normalize(cameraPos  - fragPos);
            h = normalize(l + v); 

            NdotH = dot(n, h);
            NdotL = dot(n, l);
            VdotH = dot(v, h);
            NdotV = dot(n, v);

            g = min(min(1.0, (2 * NdotH * NdotV / VdotH)), (2 * NdotH * NdotL / VdotH));
            //color = vec4(g, g, g ,1.0);
            d = MicrofacetDist(acos(NdotH));
            fresnel = fresnel0 + (1.0 - fresnel0) * pow(1 - VdotH, 5.0);
            
            vec3 diffused = (1 - ka - ks) * min(kd * max(0, NdotL), 1.0);
            vec3 specular = ks * ((fresnel * d * g) / (3.14 * NdotV * NdotL));

            color += vec4(attenuation * lights[light].lumI * (( 1 - ka -ks) * kd  + ks * (d * g * fresnel) / (3.14 * NdotV)), 1.0);
        
        }
        
        //Apply shadows
        float distToLightStored;
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
                distToLightStored = texture(shadowMap, shadowCoords.xy + poissonDisk[i]*blur).z;
                
                if(distToLightStored < shadowCoords.z + bias) {
                    visibility -= 0.25;
                }
            }
        }
        
        //distToLightStored = texture(shadowMap, shadowCoords.xy).z;
        //color = vec4(distToLightStored, distToLightStored, distToLightStored, 1.0);
        //color = vec4(shadowCoords.z, shadowCoords.z, shadowCoords.z, 1.0);
        color *= visibility;
    }
}
