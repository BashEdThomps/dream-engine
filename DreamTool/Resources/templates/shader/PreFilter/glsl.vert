/* 
* Dream
* Shader Template: PBR
* GLSL 330 Core
*/


#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 WorldPos;

uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;

void main()
{
    WorldPos = aPos;  
    gl_Position =  uProjectionMatrix * uViewMatrix * vec4(WorldPos, 1.0);
}