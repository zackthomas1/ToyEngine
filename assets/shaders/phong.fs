#version 330 core

in VS_OUT
{
    vec3 fragPos; 
    vec3 normal; 
    vec2 texCoords;
} fs_in;

struct Material{
    // Note: sampler2D is an opaque type and can only be used as a uniform.
    // Instantiating this struct outside of a uniform may cause GLSL errors.
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
}; uniform Material material;

struct DirectionalLight{ 
    vec3 direction; // directional vector replaces position vector
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    bool enabled; // flag to enable/disable this light
}; 

struct PointLight{
    vec3 position; 
    vec3 ambient; 
    vec3 diffuse; 
    vec3 specular; 
    float constant; 
    float linear; 
    float quadratic;
    bool enabled; 
}

struct SpotLight{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
    vec3 spotDirection;
    float innerAngle;
    float outerAngle;
    bool enabled;
}
#define MAX_DIRECTIONAL_LIGHTS 4
#define MAX_POINT_LIGHTS 32
#define MAX_SPOT_Lights 16

uniform DirectionalLight uDirectionalLights[MAX_DIRECTIONAL_LIGHTS];
uniform PointLight uPointLights[MAX_POINT_LIGHTS]; 
uniform SpotLight uSpotLights[MAX_SPOT_LIGHTS]

uniform int uNumDirectionalLights;
uniform int uNumPointLights;
uniform int uNumberSpotLights;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 fragPos);
vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);

uniform vec3 viewPos;

out vec4 FragColor;

/*
 * Fragment shader main function for Phong shading.
 *
 * - Normalizes the input normal vector.
 * - Calculates the view direction from the fragment position to the viewer position.
 * - Computes the output color using directional lighting via the CalcDirLight function.
 * - Sets the final fragment color (FragColor) with the computed color and full opacity.
 *
 * Assumes the following uniforms/inputs are defined elsewhere:
 *   - normal: The interpolated normal vector at the fragment.
 *   - fragPos: The position of the fragment in world space.
 *   - viewPos: The position of the camera/viewer in world space.
 *   - dirLight: The properties of the directional light source.
 *   - CalcDirLight: Function that computes lighting based on the Phong model.
 */
void main()
{
    vec3 norm = normalize(fs_in.normal); 
    vec3 viewDir = normalize(fs_in.fragPos - viewPos);

    vec3 result = vec3(0.0);

    // directional light
    for (int i = 0; i < uNumDirectionalLights and i < MAX_DIRECTIONAL_LIGHTS; i++){
        if(uDirectionalLights[i].enabled){
            result += CalcDirLight(uDirectionalLights[i], norm, viewDir)
        }
    }
    
    // point light
    for (int i = 0; i < uNumPointLights and i < MAX_POINT_LIGHTS; i++){
        if(uPointLights[i].enabled){
            result += CalcPointLight(uPointLights[i], norm, viewDir, fragPos)
        }
    }

    // spot light
    for (int i = 0; i < uNumberSpotLights and i < MAX_SPOT_Lights; i++){
        if(uSpotLights[i].enabled){
            result += CalcSpotLight(uSpotLights[i], norm, viewDir, fragPos)
        }
    }

    FragColor = vec4(result, 1.0);
}

/**
 * Calculates the lighting contribution from a directional light source using the Phong reflection model.
 *
 * @param light    The directional light source, containing direction, ambient, diffuse, and specular components.
 * @param normal   The surface normal vector at the fragment.
 * @param viewDir  The direction from the fragment to the viewer (camera).
 *
 * @return The resulting color from the combination of ambient, diffuse, and specular lighting components.
 *
 * The function performs the following steps:
 * 1. Computes the direction of the incoming light.
 * 2. Samples the diffuse texture color at the current fragment.
 * 3. Calculates the ambient component by multiplying the diffuse texture color with the light's ambient color.
 * 4. Calculates the diffuse component using the Lambertian reflectance model.
 * 5. Calculates the specular component using the Phong reflection model and the specular texture.
 * 6. Returns the sum of ambient, diffuse, and specular components as the final color.
 */
vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    // Light direction (from fragment to light)
    vec3 lightDir = normalize(-light.direction);

    // Diffuse texture color
    vec3 diffuseTexColor = texture(material.texture_diffuse1, fs_in.texCoords).rgb;
    
    // Ambient: texture modulated by ambient light
    vec3 ambientColor = diffuseTexColor * light.ambient;
    
    // Diffuse: Lambertian reflectance
    float cosineTerm = max(dot(normal, lightDir), 0.0);
    vec3 diffuseColor = diffuseTexColor * cosineTerm * light.diffuse;
    
    // Specular: Phong reflection with texture
    // Calculates angular distance between reflection direction and view direction.
    // Smaller angular distance result in greater specular light contribute.
    vec3 reflectDir = reflect(-lightDir, normal);
    float specularIntensity = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
    vec3 specularColor = texture(material.texture_specular1, fs_in.texCoords).rgb * specularIntensity * light.specular;

    return (ambientColor + diffuseColor + specularColor);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos)
{
    // light caster
    // ------------------
    vec3 lightDir = normalize(light.position - fragPos);    // positional light caster

    float distance = distance(light.position, fragPos);
    float attenuation = 1.0 / (light.constant + (light.linear * distance) + (light.quadratic * pow(distance, 2)));

    // ambient color
    // ------------------
    vec3 ambientColor = texture(material.diffuse, texCoords).rgb * light.ambient;

    // diffuse color
    // -----------------
    // Note: The cosine term is the factor that describes how much light interacts with surface. 
    // A fragments brightness increases the closer it aligns with the incoming light rays from the source.
    float cosineTerm = max(dot(normal, lightDir), 0.0);
    vec3 diffuseColor = texture(material.diffuse, texCoords).rgb * cosineTerm * light.diffuse;

    // specular color
    // -------------------

    // Note: The lightDir vector is negated. The reflect function expects the first vector 
    // to point from the light source towards the fragment's position. The lightDir vector 
    // currently points the other way around. To make sure we get the correct reflect vector
    // we reverse the lightDir vector. 
    vec3 reflectDir = reflect(-lightDir, normal); 

    // Note: Calculate the angular distance between this reflection vector and the view direction.
    // The closer the angle between them, the greater the impact of the specular light.
    float specularIntensity = pow(max(dot(reflectDir, viewDir), 0.0),material.shininess);
    vec3 specularColor = (texture(material.specular, texCoords).rgb * specularIntensity) * light.specular;

    ambientColor    *= attenuation;
    diffuseColor    *= attenuation;
    specularColor   *= attenuation;

    return ambientColor + diffuseColor + specularColor;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 fragPos){
    // light caster
    // ------------------
    float distance = distance(light.position, fragPos);
    float attenuation = 1.0 / (light.constant + (light.linear * distance) + (light.quadratic * pow(distance, 2)));
    
    vec3 lightDir = normalize(light.position - fragPos);    // positional light caster

    // calculate spot light terms
    float theta = dot(lightDir, normalize(-light.spotDir)); // the angle between the light direction and spot direction in radians
    float epsilon = light.innerAngle - light.outerAngle;
    float intensity = clamp((theta - light.outerAngle) / epsilon, 0.0, 1.0);

    // ambient color
    // ------------------
    vec3 ambientColor = texture(material.diffuse, texCoords).rgb * light.ambient;

    // diffuse color
    // -----------------   
    float cosineTerm = max(dot(normal, lightDir), 0.0);
    vec3 diffuseColor = texture(material.diffuse, texCoords).rgb * cosineTerm * light.diffuse;

    // specular color
    // -------------------
    vec3 reflectDir = reflect(-lightDir, normal); 

    float specularIntensity = pow(max(dot(reflectDir, viewDir), 0.0),material.shininess);
    vec3 specularColor = (texture(material.specular, texCoords).rgb * specularIntensity) * light.specular;

    diffuseColor    *= intensity;
    specularColor   *= intensity;

    ambientColor    *= attenuation;
    diffuseColor    *= attenuation;
    specularColor   *= attenuation;

    return ambientColor + diffuseColor + specularColor;
}