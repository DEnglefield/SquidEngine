#version 330 core
out vec4 FragColor;

uniform sampler2D textureIn;

struct Material {
    vec4 colour;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float reflectivity;
}; 

uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform Material material;

in vec3 fragPos;
in vec3 fragNormal;
in vec2 texUV;
  

void main() {
    vec3 lightColour = vec3(1.0f, 1.0f, 1.0f);

    vec3 ambientLight = material.ambient * lightColour;

    vec3 lightVec = normalize(lightPos - fragPos);
    float lightAngle = clamp(dot(fragNormal, lightVec),0.0,1.0);
    vec3 diffuseLight = lightAngle * lightColour;

    vec3 viewVec = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-viewVec, fragNormal);
    float specValue = max(dot(viewVec, reflectDir), 0.0);
    vec3 specularLight = pow(specValue, material.reflectivity) * material.specular * lightColour; 

    vec3 blinnPhong = ambientLight + diffuseLight + specularLight;

	FragColor = vec4(blinnPhong,1) * material.colour * texture(textureIn, texUV);
} 