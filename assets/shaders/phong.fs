#version 330 core

#define MAX_LIGHTS 12

#define AMBIENT_INFLUENCE 0.2
#define DIFFUSE_INFLUENCE 1
#define SPECULAR_INFLUENCE 0.5

// Recommended default attenuation values for realistic quadratic light falloff
#define CONSTANT_ATTEN 1.0
#define LINEAR_ATTEN 0.09
#define QUADRATIC_ATTEN 0.032

in VS_OUT{
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
    vec3 viewPos;
} fs_in;

struct Material{
    // Note: sampler2D is an opaque type and can only be used as a uniform.
    // Instantiating this struct outside of a uniform may cause GLSL errors.
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    samplerCube environment_map; 
    bool has_environment_map;
    float roughness;
    float metallic;
    float transmission;
    float refractive_index;
}; uniform Material material;

uniform int uMaxMipLevel;

// Each Light struct is padded to a multiple of 16 bytes (total 80 bytes)
struct Light{
                        // base alignment   // aligned offset
    int type;           // 4 bytes          0 bytes
    int enabled;        // 4 bytes          4 bytes 
    float innerAngle;   // 4 bytes          8 bytes
    float outerAngle;   // 4 bytes          12 bytes

    vec4 value;         // 16 bytes         16 bytes
    vec4 direction;     // 16 bytes         32 bytes
    vec4 position;      // 16 bytes         48 bytes
    vec4 spotDirection; // 16 bytes         64 bytes

    // NOTE: The inner and outer angles are measured in terms of their cosine value.
};

// - The block itself is aligned to 16 bytes
layout (std140) uniform LightBlock{
    Light uLights[MAX_LIGHTS];  // offset 0, each element aligned to 16 bytes
    int uNumLight;              // offset , base alignment 4, but next member must start at 16 (vec4 boundary)
};

vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir, vec3 diffuseTex, vec3 specularTex);
vec3 CalcPointLight(Light light, vec3 normal, vec3 viewDir, vec3 fragPos, vec3 diffuseTex, vec3 specularTex);
vec3 CalcSpotLight(Light light, vec3 normal, vec3 viewDir, vec3 fragPos, vec3 diffuseTex, vec3 specularTex);

vec3 CalcSpecularColor(Light light, vec3 normal, vec3 lightDir, vec3 viewDir, vec3 specularTex);
vec3 CalcReflection(vec3 normal, vec3 viewDir); 
vec3 CalcRefraction(vec3 normal, vec3 viewDir); 
float CalcFresnel(vec3 normal, vec3 viewDir, float refractive_index);

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
    vec3 viewDir = normalize(fs_in.viewPos - fs_in.fragPos);    // from fragment to camera

    // texture samples
    vec3 diffuseTex = texture(material.texture_diffuse1, fs_in.texCoords).rgb;
    vec3 specularTex = texture(material.texture_specular1, fs_in.texCoords).rgb;

    vec3 result = vec3(0.0);
    for (int i = 0; i < uNumLight && i < MAX_LIGHTS; ++i){
        Light light = uLights[i]; 
        if(light.enabled == 0) continue;

        switch(light.type){
            case 1:
                result += CalcDirLight(light, norm, viewDir, diffuseTex, specularTex);
                break;
            case 2:
                result += CalcPointLight(light, norm, viewDir, fs_in.fragPos, diffuseTex, specularTex);
                break;
            case 3:
                result += CalcSpotLight(light, norm, viewDir, fs_in.fragPos, diffuseTex, specularTex);
                break;
            default:
                break;
        }
    }

    if(material.has_environment_map && (material.metallic > 0.001 || material.transmission > 0.001))
    {
        // Calculate reflection once
        vec3 reflection     = CalcReflection(norm, viewDir);
        
        // Handle transmission (glass-like materials)
        if (material.transmission > 0.001) 
        {
            vec3 refraction = vec3(0.0f);
            refraction      = CalcRefraction(norm, viewDir);
            float fresnel   = CalcFresnel(norm, viewDir, material.refractive_index);
            refraction      = mix(refraction, reflection, fresnel);
            result          = mix(result, refraction, material.transmission);
        }

        // Handle Metallic (mirror-like)
        if (material.metallic > 0.001)
        {
            // For metals, replace diffuse with reflection
            result = mix(result, reflection, material.metallic);
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
vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir, vec3 diffuseTex, vec3 specularTex)
{
    // Light direction (from fragment to light)
    vec3 lightDir = normalize(-light.direction.xyz);

    // Ambient: texture modulated by ambient light
    vec3 ambientColor = diffuseTex * (light.value.xyz * AMBIENT_INFLUENCE);

    // Diffuse: Lambertian reflectance
    float cosineTerm = max(dot(normal, lightDir), 0.0);
    vec3 diffuseColor = diffuseTex * cosineTerm * (light.value.xyz * DIFFUSE_INFLUENCE);
    
    // Specular: Phong reflection with texture
    // Calculates angular distance between reflection direction and view direction.
    // Smaller angular distance result in greater specular light contribute.
    vec3 specularColor = CalcSpecularColor(light, normal, lightDir, viewDir, specularTex);

    return (ambientColor + diffuseColor + specularColor);
}

vec3 CalcPointLight(Light light, vec3 normal, vec3 viewDir, vec3 fragPos, vec3 diffuseTex, vec3 specularTex)
{
    // light caster
    // ------------------
    vec3 lightDir = normalize(light.position.xyz - fragPos);

    float distance = distance(light.position.xyz, fragPos);
    float attenuation = 1.0 / (CONSTANT_ATTEN + (LINEAR_ATTEN * distance) + (QUADRATIC_ATTEN * pow(distance, 2)));

    // ambient color
    // ------------------
    vec3 ambientColor = diffuseTex * (light.value.xyz * AMBIENT_INFLUENCE);

    // diffuse color
    // -----------------
    // Note: The cosine term is the factor that describes how much light interacts with surface. 
    // A fragments brightness increases the closer it aligns with the incoming light rays from the source.
    float cosineTerm    = max(dot(normal, lightDir), 0.0);
    vec3 diffuseColor   = diffuseTex * cosineTerm * (light.value.xyz * DIFFUSE_INFLUENCE);

    // specular color
    // -------------------
    vec3 specularColor = CalcSpecularColor(light, normal, lightDir, viewDir, specularTex);

    ambientColor    *= attenuation;
    diffuseColor    *= attenuation;
    specularColor   *= attenuation;

    return ambientColor + diffuseColor + specularColor;
}

vec3 CalcSpotLight(Light light, vec3 normal, vec3 viewDir, vec3 fragPos, vec3 diffuseTex, vec3 specularTex)
{
    // light caster
    // ------------------
    float distance = distance(light.position.xyz, fragPos);
    float attenuation = 1.0 / (CONSTANT_ATTEN + (LINEAR_ATTEN * distance) + (QUADRATIC_ATTEN * pow(distance, 2)));

    vec3 lightDir = normalize(light.position.xyz - fragPos);    // positional light caster

    // calculate spot light terms
    float theta = dot(lightDir, normalize(-light.spotDirection.xyz)); // cosine of the angle between the light direction and spot direction
    float epsilon = light.innerAngle - light.outerAngle;
    // Ensure epsilon is positive to avoid inverted ramp or division by zero
    if (epsilon <= 0.0) {
        epsilon = 0.0001; // Small positive value to prevent division by zero
    }
    float intensity = clamp((theta - light.outerAngle) / epsilon, 0.0, 1.0);

    // ambient color
    // ------------------
    vec3 ambientColor = diffuseTex * (light.value.xyz * AMBIENT_INFLUENCE);

    // diffuse color
    // -----------------   
    float cosineTerm = max(dot(normal, lightDir), 0.0);
    vec3 diffuseColor = diffuseTex * cosineTerm * (light.value.xyz * DIFFUSE_INFLUENCE);

    // specular color
    // -------------------
    vec3 specularColor = CalcSpecularColor(light, normal, lightDir, viewDir, specularTex);

    diffuseColor    *= intensity;
    specularColor   *= intensity;

    ambientColor    *= attenuation;
    diffuseColor    *= attenuation;
    specularColor   *= attenuation;

    return ambientColor + diffuseColor + specularColor;
}

vec3 CalcSpecularColor(Light light, vec3 normal, vec3 lightDir, vec3 viewDir, vec3 specularTex)
{   
    // Note: The lightDir vector is negated. The reflect function expects the first vector 
    // to point from the light source towards the fragment's position. The lightDir vector 
    // currently points the other way around. To make sure we get the correct reflect vector
    // we reverse the lightDir vector. 
    vec3 reflectDir = reflect(-lightDir, normal); 

    // Note: Calculate the angular distance between this reflection vector and the view direction.
    // The closer the angle between them, the greater the impact of the specular light.
    float normalizeRoughness = clamp(material.roughness / 256.0, 0.001, 1.0); 
    float specularExponent = 1 / (normalizeRoughness);
    float specularIntensity = pow(max(dot(reflectDir, viewDir), 0.0),specularExponent);

    vec3 specularColor = specularTex * specularIntensity * (light.value.xyz * SPECULAR_INFLUENCE);
    return specularColor;
}

vec3 CalcReflection(vec3 normal, vec3 viewDir)
{
    if(!material.has_environment_map){
        return vec3(0.0);
    }

    vec3 reflect_dir = reflect(-viewDir, normal);
    
    // calculate mip level based on roughness (range[0.1, 256])
    // map roughness to mip levels [0 to maxMipLevels]
    float normalizeRoughness = clamp(material.roughness / 256.0, 0.001, 1.0); 
    float mipLevel = (normalizeRoughness * normalizeRoughness) * float(uMaxMipLevel);

    return textureLod(material.environment_map, reflect_dir, mipLevel).rgb;
}

vec3 CalcRefraction(vec3 normal, vec3 viewDir)
{
    if(!material.has_environment_map){
        return vec3(0.0);
    }

    vec3 refraction_dir = refract(-viewDir, normal, 1.0 / material.refractive_index);

    // Handle total internal reflections
    if(length(refraction_dir) < 0.001){
        return CalcReflection(normal, viewDir);
    }

    float normalizeRoughness = clamp(material.roughness / 256.0, 0.001, 1.0); 
    float mipLevel = (normalizeRoughness * normalizeRoughness) * float(uMaxMipLevel);

    return textureLod(material.environment_map, refraction_dir, mipLevel).rgb;
}

float CalcFresnel(vec3 normal, vec3 viewDir, float refractive_index)
{
    float cosTheta = max(dot(normal, viewDir), 0.0); 
    float r0 = pow((1.0 - refractive_index) / (1.0 + refractive_index), 2.0); 
    return r0 + (1.0 - r0) * pow(1.0 - cosTheta, 5.0); 
}