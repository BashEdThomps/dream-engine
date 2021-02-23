/* 
* Dream
* Shader Template: Shadow Pass
* GLSL 330 Core
*/

#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 uLightSpaceMatrix;
uniform mat4 uModelArray[100];

void main()
{
    gl_Position = uLightSpaceMatrix * uModelArray[gl_InstanceID] * vec4(aPos, 1.0);
}

