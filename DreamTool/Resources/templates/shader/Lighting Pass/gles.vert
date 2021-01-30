#version 300 es

// Lighting Pass

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 shadowSpaceMatrix;

out vec2 TexCoords;
out mat4 ShadowSpaceMatrix;

void main()
{
    TexCoords = aTexCoords;
    ShadowSpaceMatrix = shadowSpaceMatrix;
    gl_Position = vec4(aPos, 1.0);
}
