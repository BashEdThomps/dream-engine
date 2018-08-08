#version 330 core

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;


out vec4 color;

uniform sampler2D texture_diffuse1;

uniform vec3  ambientColour;
uniform float ambientStrength;

uniform vec3 diffuseColour;
uniform vec3 specularColour;

uniform int numPointLights;
uniform vec3 pointLightPos[10];
uniform vec3 pointLightColour[10];

vec3 calculateDiffuse(vec3 normal, vec3 lightPos,vec3 lightColour, vec3 fragPos)
{
    vec3 lightDir = normalize(lightPos-fragPos);
    float diff = max(dot(normal,lightDir), 0.0);
    vec3 diffuse = diff * lightColour;
    return diffuse;
}

void main()
{
    vec3  specularUnused = specularColour;
    vec4 objectColor = vec4(texture(texture_diffuse1, TexCoords));

    // Don't use 'transparent' fragments
    if(objectColor.a < 0.5)
            discard;

    // Calculate Ambient
    vec3 ambient = vec3(ambientStrength * ambientColour);

    // Calculate Diffuse
    vec3 norm = normalize(Normal);
    vec3 diffuseLightSum = vec3(0);
    vec3 diffuseSum = diffuseColour;
    if (numPointLights > 0)
    {
        for(int i=0; i<numPointLights; i++)
        {
                    diffuseLightSum = diffuseLightSum + calculateDiffuse(norm, pointLightPos[i], pointLightColour[i], FragPos);
        }
        diffuseLightSum = diffuseLightSum / numPointLights;
        diffuseSum = diffuseSum * diffuseLightSum;
    }
    // Combine Ambient & diffuse
    color = vec4((ambient + diffuseSum),1.0f) * objectColor;
}


