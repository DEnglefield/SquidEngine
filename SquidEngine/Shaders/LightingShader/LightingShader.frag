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
    float opacity;
    sampler2D diffuseMaps[MAX_DIFFUSE_MAPS];
    sampler2D specularMaps[MAX_SPECULAR_MAPS];
    
}; 


struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float linearFallOff;
    float quadraticFallOff;
}; 


struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float linearFallOff;
    float quadraticFallOff;
    float innerCutOff;
    float outerCutOff;
}; 


struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}; 


uniform int numPointLights;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform int numSpotLights;
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform int numDirectionalLights;
uniform DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];

uniform Material material;

in vec3 fragPos;
in vec3 fragNormal;
in vec2 texUV;

uniform vec3 cameraPos;

vec3 getAmbientLight();
vec3 getDiffuseLight(vec3 lightVec);
vec3 getSpecularLight(vec3 lightVec, vec3 viewVec);
float getAttenuation(vec3 lightPos, float linearFallOff, float quadraticFallOff);

vec3 applyPointLight(PointLight light, vec3 viewVec);
vec3 applySpotLight(SpotLight light, vec3 viewVec);
vec3 applyDirectionalLight(DirectionalLight light, vec3 viewVec); 


void main() {

    vec3 lighting = vec3(0,0,0);

    vec3 viewVec = normalize(cameraPos - fragPos);

    for (int i=0; i < numPointLights; ++i){
        lighting += applyPointLight(pointLights[i], viewVec);
    }   

    for (int i=0; i < numSpotLights; ++i){
        lighting += applySpotLight(spotLights[i], viewVec);
    }   

    for (int i=0; i < numDirectionalLights; ++i){
        lighting += applyDirectionalLight(directionalLights[i], viewVec);
    }  

   
   FragColor = vec4(lighting, material.opacity);

} 


vec3 applyPointLight(PointLight light, vec3 viewVec){

    vec3 lightVec = normalize(light.position - fragPos);
    
    vec3 ambientLight = getAmbientLight() * light.ambient;
    vec3 diffuseLight = getDiffuseLight(lightVec) * light.diffuse;
    vec3 specularLight = getSpecularLight(lightVec,viewVec) * light.specular;

    float attenuation = getAttenuation(light.position, light.linearFallOff, light.quadraticFallOff); 
    ambientLight *= attenuation;
    diffuseLight *= attenuation;
    specularLight *= attenuation;
    return (ambientLight + diffuseLight + specularLight);
}



vec3 applySpotLight(SpotLight light, vec3 viewVec){

    vec3 lightVec = normalize(light.position - fragPos);
    

    vec3 ambientLight = getAmbientLight() * light.ambient;
    vec3 diffuseLight = getDiffuseLight(lightVec) * light.diffuse;
    vec3 specularLight = getSpecularLight(lightVec,viewVec) * light.specular;
   
    float attenuation = getAttenuation(light.position, light.linearFallOff, light.quadraticFallOff); 

    float fragmentAngle = dot(lightVec, normalize(-light.direction));
    float epsilon = light.innerCutOff - light.outerCutOff;
    float intensity = clamp((fragmentAngle - light.outerCutOff) / epsilon, 0.0, 1.0);

    ambientLight *= attenuation * intensity;
    diffuseLight *= attenuation * intensity;
    specularLight *= attenuation * intensity;

    return (ambientLight + diffuseLight + specularLight);
 
}


vec3 applyDirectionalLight(DirectionalLight light, vec3 viewVec){

    vec3 lightVec = normalize(-light.direction);

    vec3 ambientLight = getAmbientLight() * light.ambient;
    vec3 diffuseLight = getDiffuseLight(lightVec) * light.diffuse;
    vec3 specularLight = getSpecularLight(lightVec,viewVec) * light.specular;

    return (ambientLight + diffuseLight + specularLight);
}


vec3 getAmbientLight() { 
    vec3 diffuseMapping = vec3(1,1,1);
    for (int i=0; i < material.numDiffuseMaps; ++i){ diffuseMapping *= texture(material.diffuseMaps[i], texUV).xyz; }
    return diffuseMapping;
}

vec3 getDiffuseLight(vec3 lightVec) {
    float lightAngle = clamp(dot(fragNormal, lightVec),0.0,1.0);
    return lightAngle * getAmbientLight();
}

vec3 getSpecularLight(vec3 lightVec, vec3 viewVec) {
    vec3 reflectDir = reflect(-lightVec, fragNormal);
    vec3 halfwayDir = normalize(lightVec + viewVec);
    float specValue = max(dot(fragNormal, halfwayDir), 0.0);
    //float specValue = max(dot(viewVec, reflectDir), 0.0);
    vec3 specularMapping = vec3(1,1,1);;
    for (int i=0; i < material.numSpecularMaps; ++i){ specularMapping *= texture(material.specularMaps[i], texUV).xyz; }
    return specularMapping * pow(specValue, material.highlight);
}


float getAttenuation(vec3 lightPos, float linearFallOff, float quadraticFallOff){
    float fragDistance = length(lightPos - fragPos);
    return 1.0 / (1.0f + linearFallOff * fragDistance + quadraticFallOff * pow(fragDistance,2)); 
}