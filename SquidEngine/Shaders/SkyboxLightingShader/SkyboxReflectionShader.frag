#version 330 core
out vec4 FragColor;

#define MAX_POINT_LIGHTS 8
#define MAX_SPOT_LIGHTS 8
#define MAX_DIRECTIONAL_LIGHTS 8

#define MAX_DIFFUSE_MAPS 8 
#define MAX_SPECULAR_MAPS 8

struct Material {
    int numDiffuseMaps;
    int numSpecularMaps;
    float highlight;
    float reflectivity;
    float refractivity;
    float refractiveIndex;
    float opacity;
    sampler2D diffuseMaps[MAX_DIFFUSE_MAPS];
    sampler2D specularMaps[MAX_SPECULAR_MAPS];
}; 


uniform Material material;

uniform samplerCube skyboxTexture;

in vec3 fragPos;
in vec3 fragNormal;
in vec2 texUV;

uniform vec3 cameraPos;

vec3 applySkyboxReflection(vec3 viewVec);
vec3 applySkyboxRefraction(vec3 viewVec);

void main() {

    vec3 skyBoxLighting = vec3(0,0,0);

    vec3 viewVec = normalize(fragPos - cameraPos);

    skyBoxLighting += applySkyboxReflection(viewVec);
    skyBoxLighting += applySkyboxRefraction(viewVec);

    FragColor = vec4(skyBoxLighting,material.opacity);
    //FragColor = vec4(0,1,0,1);
} 


vec3 applySkyboxReflection(vec3 viewVec){
    vec3 reflectVec = reflect(viewVec, normalize(fragNormal));
    return texture(skyboxTexture, reflectVec).rgb * material.reflectivity;
}


vec3 applySkyboxRefraction(vec3 viewVec){
    float ratio = 1/material.refractiveIndex;
    vec3 refractVec = refract(viewVec, normalize(fragNormal), ratio);
    return texture(skyboxTexture, refractVec).rgb * (1-material.opacity);
}