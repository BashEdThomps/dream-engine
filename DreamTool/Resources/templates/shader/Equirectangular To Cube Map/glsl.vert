/*
    Dream
    Shader Template: Equirectangular to Cube Map
    GLSL 330 Core
*/
#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 localPos;

uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;

void main()
{
    localPos = aPos;  
    gl_Position =  uProjectionMatrix * uViewMatrix * vec4(localPos, 1.0);
}