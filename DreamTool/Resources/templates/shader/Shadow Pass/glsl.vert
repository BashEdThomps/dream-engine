#version 330 core

// Shadow Pass 

layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model[100];

void main()
{
    gl_Position = lightSpaceMatrix * model[gl_InstanceID] * vec4(aPos, 1.0);
}

