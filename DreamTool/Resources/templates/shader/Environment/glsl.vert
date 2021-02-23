/*
    Dream
    Shader Template: Environment Cube
    GLSL 330 Core
*/

#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;

out vec3 WorldPos;

void main()
{
    WorldPos = aPos;

	mat4 rotView = mat4(mat3(uViewMatrix));
	vec4 clipPos = uProjectionMatrix * rotView * vec4(WorldPos, 1.0);

	gl_Position = clipPos.xyww;
}