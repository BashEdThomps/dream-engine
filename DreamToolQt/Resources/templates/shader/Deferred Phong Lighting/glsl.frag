#version 330 core
out vec4 FragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct DirLight {
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
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define MAX_LIGHTS 10
uniform vec3 viewPos;

in vec2 TexCoords;

uniform int pointLightCount;
uniform int spotLightCount;
uniform int directionalLightCount;

uniform DirLight dirLights[MAX_LIGHTS];
uniform PointLight pointLights[MAX_LIGHTS];
uniform SpotLight spotLights[MAX_LIGHTS];

vec3  FragPos = vec3(0.0);
vec3  ViewDir = vec3(0.0);
vec3  Normal  = vec3(0.0);
vec3  Albedo  = vec3(0.0);
float Specular = 0.0;

// function prototypes
vec3 CalcDirLight(DirLight light);
vec3 CalcPointLight(PointLight light);
vec3 CalcSpotLight(SpotLight light);

void main()
{
    // properties
    // retrieve data from G-buffer
    FragPos = texture(gPosition, TexCoords).rgb;
    Normal = texture(gNormal, TexCoords).rgb;
    Albedo = texture(gAlbedoSpec, TexCoords).rgb;
    Specular = texture(gAlbedoSpec, TexCoords).a;
    ViewDir = normalize(viewPos - FragPos);

    // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================

    vec3 result = vec3(0.0);

    // phase 1: directional lighting
    for(int i = 0; i < directionalLightCount; i++)
        result += CalcDirLight(dirLights[i]);

    // phase 2: point lights
    for(int i = 0; i < pointLightCount; i++)
        result += CalcPointLight(pointLights[i]);

    // phase 3: spot light
    for(int i = 0; i < spotLightCount; i++)
        result += CalcSpotLight(spotLights[i]);

    FragColor = vec4(result, 1.0);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(Normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, Normal);
    float spec = pow(max(dot(ViewDir, reflectDir), 0.0), Specular);
    // combine results
    vec3 ambient = light.ambient * Albedo;
    vec3 diffuse = light.diffuse * diff * Albedo;
    vec3 specular = light.specular * spec * vec3(Specular);
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light)
{
    vec3 lightDir = normalize(light.position - FragPos);
    // diffuse shading
    float diff = max(dot(Normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, Normal);
    float spec = pow(max(dot(ViewDir, reflectDir), 0.0), Specular);
    // attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient * Albedo;
    vec3 diffuse = light.diffuse * diff * Albedo;
    vec3 specular = light.specular * spec * vec3(Specular);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light)
{
    vec3 lightDir = normalize(light.position - FragPos);
    // diffuse shading
    float diff = max(dot(Normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, Normal);
    float spec = pow(max(dot(ViewDir, reflectDir), 0.0), Specular);
    // attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * Albedo;
    vec3 diffuse = light.diffuse * diff * Albedo;
    vec3 specular = light.specular * spec;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}
