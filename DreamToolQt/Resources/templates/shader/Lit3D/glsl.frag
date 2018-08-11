#version 330 core

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;


out vec4 color;

uniform sampler2D texture_diffuse1;

uniform vec3  worldAmbientColour;
uniform float worldAmbientStrength;

uniform vec3  materialAmbientColour;
uniform float materialAmbientStrength;

uniform vec3 materialDiffuseColour;
uniform float materialDiffuseStrength;

uniform vec3 materialSpecularColour;
uniform float materialSpecularStrength;

uniform int numPointLights;
uniform vec3 pointLightPos[10];
uniform vec3 pointLightColour[10];

vec3 calculatePointLight(vec3 normal, vec3 lightPos,vec3 lightColour, vec3 fragPos)
{
    vec3 lightDirection = normalize(lightPos-fragPos);
    float intensity = max(dot(normal,lightDirection), 0.0);
    vec3 lightFinal = lightColour * intensity;
    return lightFinal;
}

void main()
{
    vec4 objectColor = vec4(texture(texture_diffuse1, TexCoords));
    vec3 materialSpecular = materialSpecularColour * materialSpecularStrength;
    vec3 materialDiffuse = materialDiffuseColour * materialDiffuseStrength;
    vec3 materialAmbient = materialAmbientColour * materialAmbientStrength;
    vec3 worldAmbient = worldAmbientStrength * worldAmbientColour;

    // Don't use 'transparent' fragments
    if(objectColor.a < 0.5)
        discard;

    vec3 colourSum = materialDiffuse + materialAmbient + materialSpecular + worldAmbient;

    // Calculate Diffuse
    vec3 norm = normalize(Normal);

    if (numPointLights > 0)
    {
    vec3 pointLightSum = vec3(0);
        for(int i=0; i<numPointLights; i++)
        {
            pointLightSum = pointLightSum + calculatePointLight(norm, pointLightPos[i], pointLightColour[i], FragPos);
        }
        pointLightSum = pointLightSum / numPointLights;
        colourSum = colourSum + pointLightSum;
    }

    vec3 colourSumNormalised = normalize(colourSum);

    // Combine with object colour
    color = vec4(colourSumNormalised,1.0f) * objectColor;
}


