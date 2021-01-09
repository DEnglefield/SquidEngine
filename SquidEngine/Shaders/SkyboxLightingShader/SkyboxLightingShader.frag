#version 330 core
out vec4 FragColor;

//Shader used to implement blinn-phong lighting and sample from skybox reflections

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
    float opacity;
    sampler2D diffuseMaps[MAX_DIFFUSE_MAPS];
    sampler2D specularMaps[MAX_SPECULAR_MAPS];
}; 

//Point light definition
struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float linearFallOff;
    float quadraticFallOff;
}; 


//Spot light definition
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


//Directional lght definition
struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}; 


//Point light instances
uniform int numPointLights;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

//Spot light instances
uniform int numSpotLights;
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

//Directional light instances
uniform int numDirectionalLights;
uniform DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];

//Current material instance
uniform Material material;

//Skybox reflection map (1:1 scale)
uniform sampler2D skyboxReflections;

//Shadow map
uniform sampler2D shadowMap;

in vec3 fragPos;
in vec3 fragNormal;
in vec2 texUV;
in vec4 lightFragPos;

uniform vec3 cameraPos;

vec3 getAmbientLight();
vec3 getDiffuseLight(vec3 lightVec);
vec3 getSpecularLight(vec3 lightVec, vec3 viewVec);
float getAttenuation(vec3 lightPos, float linearFallOff, float quadraticFallOff);

vec3 applyPointLight(PointLight light, vec3 viewVec);
vec3 applySpotLight(SpotLight light, vec3 viewVec);
vec3 applyDirectionalLight(DirectionalLight light, vec3 viewVec); 

bool isInShadow();

void main() {
    vec3 lighting = vec3(0,0,0);

    //Get vector of sight
    vec3 viewVec = normalize(cameraPos - fragPos);
   
    //Average all point light illumination
    for (int i=0; i < numPointLights; ++i){
        lighting += applyPointLight(pointLights[i], viewVec);
    }   

    //Average all spot light illumination
    for (int i=0; i < numSpotLights; ++i){
        lighting += applySpotLight(spotLights[i], viewVec);
    }   

    //Average all directional light illumination
    for (int i=0; i < numDirectionalLights; ++i){
        lighting += applyDirectionalLight(directionalLights[i], viewVec);
    }  
    
    //vec2 texcoord = ((gl_FragCoord.xy - adjust.xy) / scale.xy) / textureSize(sampler0); 

    //Sample skybox reflection map
    lighting += texelFetch( skyboxReflections, ivec2(gl_FragCoord.xy), 0 ).rgb;

    //Output total light
    FragColor = vec4(lighting, material.opacity); 
 
    //FragColor = vec4(vec3(texture(shadowMap,texUV).r), 1); 
} 


//Calculate illumination for a single point light
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


//Calculate illumination for a single spot light
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


//Calculate illumination for a single directional light
vec3 applyDirectionalLight(DirectionalLight light, vec3 viewVec){

    vec3 lightVec = normalize(-light.direction);

    vec3 ambientLight = getAmbientLight() * light.ambient;
    vec3 diffuseLight = getDiffuseLight(lightVec) * light.diffuse;
    vec3 specularLight = getSpecularLight(lightVec,viewVec) * light.specular;

    return (ambientLight + diffuseLight + specularLight);
}


//Calculate illumination for ambient light for a single light source
vec3 getAmbientLight() { 
    vec3 diffuseMapping = vec3(1,1,1);
    for (int i=0; i < material.numDiffuseMaps; ++i){ diffuseMapping *= texture(material.diffuseMaps[i], texUV).xyz; }
    return diffuseMapping;
}


//Calculate illumination for diffuse light for a single light source
vec3 getDiffuseLight(vec3 lightVec) {
    float lightAngle = clamp(dot(fragNormal, lightVec),0.0,1.0);
    return lightAngle * getAmbientLight();
}


//Calculate illumination for specular light for a single light source
vec3 getSpecularLight(vec3 lightVec, vec3 viewVec) {

    vec3 reflectDir = reflect(-lightVec, fragNormal);
    vec3 halfwayDir = normalize(lightVec + viewVec);
    float specValue = max(dot(fragNormal, halfwayDir), 0.0);
    vec3 specularMapping = vec3(1,1,1);;
    for (int i=0; i < material.numSpecularMaps; ++i){ specularMapping *= texture(material.specularMaps[i], texUV).xyz; }
    return specularMapping * pow(specValue, material.highlight*4);
}


//Get lighting attenuation for point and spot light area
float getAttenuation(vec3 lightPos, float linearFallOff, float quadraticFallOff){
    float fragDistance = length(lightPos - fragPos);
    return 1.0 / (1.0f + linearFallOff * fragDistance + quadraticFallOff * pow(fragDistance,2)); 
}


//Check if fragment is in shadow using shadow map
bool isInShadow(){
     vec3 lightProjCoords = lightFragPos.xyz / lightFragPos.w;
     return false;

     //Test out orthographic
}