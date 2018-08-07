#version 330 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D texture_diffuse1;
uniform vec3  ambientColour;
uniform float ambientStrength;
uniform vec3 specularColour;
uniform vec3 diffuseColour;

void main()
{
    color = vec4(texture(texture_diffuse1, TexCoords)) * vec4(ambientColour.xyz,1.0);
}
