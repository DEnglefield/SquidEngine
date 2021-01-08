#version 330 core
out vec4 FragColor;

//Shader to produce a reflection map by sampling a skybox

#define MAX_POINT_LIGHTS 8
#define MAX_SPOT_LIGHTS 8
#define MAX_DIRECTIONAL_LIGHTS 8

#define MAX_DIFFUSE_MAPS 8 
#define MAX_SPECULAR_MAPS 8

//Material definition
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

//Current Material instance
uniform Material material;

//Skybox cubemap
uniform samplerCube skyboxTexture;

in vec3 fragPos;
in vec3 fragNormal;
in vec2 texUV;

uniform vec3 cameraPos;

vec3 applySkyboxReflection(vec3 viewVec);
vec3 applySkyboxRefraction(vec3 viewVec);


void main() {

    vec3 skyBoxLighting = vec3(0,0,0);

    //Calculate view vector to fragment surface
    vec3 viewVec = normalize(fragPos - cameraPos);

    //Get skybox reflection for current fragment
    skyBoxLighting += applySkyboxReflection(viewVec);
    //Get surface refraction with skybox
    skyBoxLighting += applySkyboxRefraction(viewVec);

    //Output result preserving material transparency
    FragColor = vec4(skyBoxLighting,material.opacity);
} 


//Get skybox reflections by calculating reflection vector and sampling from cubemap
vec3 applySkyboxReflection(vec3 viewVec){
    vec3 reflectVec = reflect(viewVec, normalize(fragNormal));
    return texture(skyboxTexture, reflectVec).rgb * material.reflectivity;
}


//Get surface refraction by calculating refraction vector for the given refractive index
vec3 applySkyboxRefraction(vec3 viewVec){
    float ratio = 1/material.refractiveIndex;
    vec3 refractVec = refract(viewVec, normalize(fragNormal), ratio);
    return texture(skyboxTexture, refractVec).rgb * (1-material.opacity);
}