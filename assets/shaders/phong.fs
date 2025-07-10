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
}; uniform DirectionalLight light;

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

    vec3 outputColor = CalcDirLight(light, norm, viewDir);
    FragColor = vec4(outputColor, 1.0);
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
