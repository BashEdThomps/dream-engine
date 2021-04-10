/* 
* Dream
* Shader Template: Draw Textured
* GLSL 330 core
*/
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrixArray[100];

void main()
{
    TexCoords = aTexCoords;
    WorldPos = vec3(uModelMatrixArray[gl_InstanceID] * vec4(aPos, 1.0));
    Normal = mat3(uModelMatrixArray[gl_InstanceID]) * aNormal;
    gl_Position =  uProjectionMatrix * uViewMatrix * vec4(WorldPos, 1.0);
}