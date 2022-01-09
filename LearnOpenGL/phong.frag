#version 450 core

#define POINT_LIGHT_COUNT 4  

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
    float reflectance;
    float refractance;
    float refractiveIndex;
}; 

struct DirectionalLight {
    bool enabled;
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

struct SpotLight {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float innerCutOff;
    float outerCutOff;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;  
  
uniform Material material;
uniform vec3 viewPos;
uniform DirectionalLight dirLight;
uniform PointLight pointLights[POINT_LIGHT_COUNT];
uniform SpotLight spotLight;  
uniform samplerCube skybox;


vec3 calcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    if (!light.enabled)
    {
        return vec3(0);
    }

    // Ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // Diffuse
    vec3 lightDir = normalize(-light.direction);
    float diffScalar = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diffScalar * vec3(texture(material.diffuse, TexCoords));  

    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);  
    float specScalar = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * specScalar * vec3(texture(material.specular, TexCoords));

    return (ambient + diffuse + specular);
}  


vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // Ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // Diffuse
    vec3 lightDir = normalize(light.position - fragPos);
    float diffScalar = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diffScalar * vec3(texture(material.diffuse, TexCoords));  

    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);  
    float specScalar = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * specScalar * vec3(texture(material.specular, TexCoords));

    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
        light.quadratic * (distance * distance));    

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}


vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // Ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // Diffuse
    vec3 lightDir = normalize(light.position - fragPos);
    float diffScalar = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diffScalar * vec3(texture(material.diffuse, TexCoords));  

    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);  
    float specScalar = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * specScalar * vec3(texture(material.specular, TexCoords));

    float epsilon = light.innerCutOff - light.outerCutOff;
    float theta = dot(lightDir, normalize(-light.direction));
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);    
    diffuse  *= intensity;
    specular *= intensity;

    return (ambient + diffuse + specular);
}

vec3 calcReflection(vec3 normal)
{
    vec3 viewDir = normalize(FragPos - viewPos);
    vec3 reflectDir = reflect(viewDir, normal);
    return material.reflectance * texture(skybox, reflectDir).rgb;
}

vec3 calcRefraction(vec3 normal)
{
    float ratio = 1.00 / material.refractiveIndex;
    vec3 viewDir = normalize(FragPos - viewPos);
    vec3 refractDir = refract(viewDir, normal, ratio);
    return material.refractance * texture(skybox, refractDir).rgb;
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Directional lighting
    vec3 result = calcDirLight(dirLight, norm, viewDir);

    // Point lights
    for(int i = 0; i < POINT_LIGHT_COUNT; i++)
    {
        result += calcPointLight(pointLights[i], norm, FragPos, viewDir);    
    }

    // Spot light
    result += calcSpotLight(spotLight, norm, FragPos, viewDir);    

    // Reflectance
    result += calcReflection(norm);

    // Refractance
    result += calcRefraction(norm);

    FragColor = vec4(result, texture(material.diffuse, TexCoords).a);
}