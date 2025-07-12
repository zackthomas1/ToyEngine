#version 330 core

in VS_OUT
{
    vec3 fragPos; 
    vec3 normal; 
    vec2 texCoords;
} fs_in;

// Material properties
struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};
uniform Material material;

// Light structures
struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    bool enabled;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
    bool enabled;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float innerCone;
    float outerCone;
    float constant;
    float linear;
    float quadratic;
    bool enabled;
};

// Light arrays and counts
#define MAX_DIRECTIONAL_LIGHTS 4
#define MAX_POINT_LIGHTS 32
#define MAX_SPOT_LIGHTS 16

uniform DirectionalLight u_directionalLights[MAX_DIRECTIONAL_LIGHTS];
uniform PointLight u_pointLights[MAX_POINT_LIGHTS];
uniform SpotLight u_spotLights[MAX_SPOT_LIGHTS];

uniform int u_numDirectionalLights;
uniform int u_numPointLights;
uniform int u_numSpotLights;

uniform vec3 viewPos;

out vec4 FragColor;

// Function prototypes
vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 fragPos);

void main()
{
    vec3 norm = normalize(fs_in.normal);
    vec3 viewDir = normalize(viewPos - fs_in.fragPos);
    
    vec3 result = vec3(0.0);
    
    // Calculate directional lights
    for(int i = 0; i < u_numDirectionalLights && i < MAX_DIRECTIONAL_LIGHTS; ++i)
    {
        if(u_directionalLights[i].enabled)
        {
            result += CalcDirectionalLight(u_directionalLights[i], norm, viewDir);
        }
    }
    
    // Calculate point lights
    for(int i = 0; i < u_numPointLights && i < MAX_POINT_LIGHTS; ++i)
    {
        if(u_pointLights[i].enabled)
        {
            result += CalcPointLight(u_pointLights[i], norm, viewDir, fs_in.fragPos);
        }
    }
    
    // Calculate spot lights
    for(int i = 0; i < u_numSpotLights && i < MAX_SPOT_LIGHTS; ++i)
    {
        if(u_spotLights[i].enabled)
        {
            result += CalcSpotLight(u_spotLights[i], norm, viewDir, fs_in.fragPos);
        }
    }
    
    FragColor = vec4(result, 1.0);
}

/**
 * Calculate directional light contribution
 */
vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    
    // Diffuse texture color
    vec3 diffuseTexColor = texture(material.texture_diffuse1, fs_in.texCoords).rgb;
    
    // Ambient lighting
    vec3 ambient = light.ambient * diffuseTexColor;
    
    // Diffuse lighting
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * diffuseTexColor;
    
    // Specular lighting
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.texture_specular1, fs_in.texCoords).rgb;
    
    return ambient + diffuse + specular;
}

/**
 * Calculate point light contribution
 */
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos)
{
    vec3 lightDir = normalize(light.position - fragPos);
    
    // Diffuse texture color
    vec3 diffuseTexColor = texture(material.texture_diffuse1, fs_in.texCoords).rgb;
    
    // Ambient lighting
    vec3 ambient = light.ambient * diffuseTexColor;
    
    // Diffuse lighting
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * diffuseTexColor;
    
    // Specular lighting
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.texture_specular1, fs_in.texCoords).rgb;
    
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return ambient + diffuse + specular;
}

/**
 * Calculate spot light contribution
 */
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 fragPos)
{
    vec3 lightDir = normalize(light.position - fragPos);
    
    // Diffuse texture color
    vec3 diffuseTexColor = texture(material.texture_diffuse1, fs_in.texCoords).rgb;
    
    // Ambient lighting
    vec3 ambient = light.ambient * diffuseTexColor;
    
    // Diffuse lighting
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * diffuseTexColor;
    
    // Specular lighting
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.texture_specular1, fs_in.texCoords).rgb;
    
    // Spotlight (soft edges)
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.innerCone - light.outerCone;
    float intensity = clamp((theta - light.outerCone) / epsilon, 0.0, 1.0);
    
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    ambient *= attenuation;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    
    return ambient + diffuse + specular;
}
