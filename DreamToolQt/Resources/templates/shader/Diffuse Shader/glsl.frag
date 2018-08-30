#version 330 core

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform vec3 worldAmbientColour;
uniform float worldAmbientStrength;

uniform vec4  materialAmbientColour;

uniform float materialOpacity;

uniform vec4  materialDiffuseColour;

uniform vec4  materialSpecularColour;
uniform float materialSpecularStrength;

uniform int  numPointLights;
uniform vec3 pointLightPos[10];
uniform vec3 pointLightColour[10];

vec4 calculatePointLight(vec3 normal, vec3 lightPos,vec3 lightColour, vec3 fragPos)
{
    vec3 lightDirection = normalize(lightPos-fragPos);
    float intensity = max(dot(normal,lightDirection), 0.0);
	 float dropOff = sqrt(sqrt(distance(fragPos,lightPos)));
    vec3 lightFinal = lightColour * intensity / dropOff;
    return vec4(lightFinal,1.0f);
}

void main()
{
    vec4 worldAmbient = vec4(worldAmbientColour * worldAmbientStrength,1.0);
    vec4 specular = materialSpecularColour * materialSpecularStrength;

    vec4 pointLightSum = vec4(0);
    if (numPointLights > 0)
    {
    // Calculate Diffuse
        vec3 norm = normalize(Normal);

        for(int i=0; i<numPointLights; i++)
        {
            pointLightSum = pointLightSum + calculatePointLight(norm, pointLightPos[i], pointLightColour[i], FragPos);
        }
        pointLightSum = pointLightSum / numPointLights;
        pointLightSum.a = 1.0f;
    }

    // Combine with object colour
    color = materialDiffuseColour * (worldAmbient + pointLightSum);
}


