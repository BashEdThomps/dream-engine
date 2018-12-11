#version 330 core

// Lighting Pass

out vec4 FragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gShadow;
uniform sampler2D gIgnore;

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
in mat4 ShadowSpaceMatrix;

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
vec4  FragPosShadowSpace = vec4(0.0);
float Specular = 0.0;
float Shadow = 0.0;
// function prototypes
vec3 CalcDirLight(DirLight light);
vec3 CalcPointLight(PointLight light);
vec3 CalcSpotLight(SpotLight light);
float ShadowCalculation();

void main()
{
    // properties
    // retrieve data from G-buffer
    FragPos = texture(gPosition, TexCoords).rgb;
    Normal = texture(gNormal, TexCoords).rgb;
    Albedo = texture(gAlbedoSpec, TexCoords).rgb;
    vec3 result = vec3(0.0);
	if (Albedo == vec3(0.0)) discard;

    if (texture(gIgnore,TexCoords).r == 1.0f)
    {
        result = Albedo;
    }
    else
    {
        Specular = texture(gAlbedoSpec, TexCoords).a;
        ViewDir = normalize(viewPos - FragPos);
        FragPosShadowSpace = ShadowSpaceMatrix * vec4(FragPos, 1.0);
        Shadow = ShadowCalculation();       

        // == =====================================================
        // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
        // For each phase, a calculate function is defined that calculates the corresponding color
        // per lamp. In the main() function we take all the calculated colors and sum them up for
        // this fragment's final color.
        // == =====================================================

       

        // phase 1: directional lighting
        for(int i = 0; i < directionalLightCount; i++)
            result += CalcDirLight(dirLights[i]); 

        // phase 2: point lights
        for(int i = 0; i < pointLightCount; i++)
           result += CalcPointLight(pointLights[i]);

        // phase 3: spot light 
        for(int i = 0; i < spotLightCount; i++)
           result += CalcSpotLight(spotLights[i]);
    }
    FragColor = vec4(result, 1.0);
   //FragColor = vec4(Shadow, 1.0);
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
    return ((ambient) * (1.0 - Shadow/2) + diffuse + specular );
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
    return ((ambient) * (1.0 - Shadow/2)+ diffuse + specular);
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

float ShadowCalculation()
{
    // perform perspective divide
    vec3 projCoords = FragPosShadowSpace.xyz / FragPosShadowSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    if(projCoords.z > 1.0)
        return 0.0;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(gShadow, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = 0.003;
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(gShadow, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(gShadow, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    return shadow;
}

