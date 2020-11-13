#version 330 core
out vec4 FragColor;



struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap;
    float reflectivity;
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

#define MAX_POINT_LIGHTS 8
#define MAX_SPOT_LIGHTS 8
#define MAX_DIRECTIONAL_LIGHTS 8

uniform bool enabledPointLights[MAX_POINT_LIGHTS];
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform bool enabledSpotLights[MAX_SPOT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform bool enabledDirectionalLights[MAX_DIRECTIONAL_LIGHTS];
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

    for (int i=0; i < MAX_POINT_LIGHTS; ++i){
        if (enabledPointLights[i]){
            lighting += applyPointLight(pointLights[i], viewVec);
        }
    }   

    for (int i=0; i < MAX_SPOT_LIGHTS; ++i){
        if (enabledSpotLights[i]){
            lighting += applySpotLight(spotLights[i], viewVec);
        }
    }   

    for (int i=0; i < MAX_DIRECTIONAL_LIGHTS; ++i){
        if (enabledDirectionalLights[i]){
            lighting += applyDirectionalLight(directionalLights[i], viewVec);
        }
    }  

   
   FragColor = vec4(lighting,1);
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


vec3 getAmbientLight() { return texture(material.diffuseMap,texUV).xyz; }

vec3 getDiffuseLight(vec3 lightVec) {
    float lightAngle = clamp(dot(fragNormal, lightVec),0.0,1.0);
    return lightAngle * texture(material.diffuseMap,texUV).xyz;
}

vec3 getSpecularLight(vec3 lightVec, vec3 viewVec) {
    vec3 reflectDir = reflect(-lightVec, fragNormal);
    float specValue = max(dot(viewVec, reflectDir), 0.0);
    return pow(specValue, material.reflectivity) * texture(material.specularMap,texUV).xyz; 
}


float getAttenuation(vec3 lightPos, float linearFallOff, float quadraticFallOff){
    float fragDistance = length(lightPos - fragPos);
    return 1.0 / (1.0f + linearFallOff * fragDistance + quadraticFallOff * pow(fragDistance,2)); 
}