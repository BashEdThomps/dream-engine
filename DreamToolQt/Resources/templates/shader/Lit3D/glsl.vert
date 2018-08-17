#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform vec3 cameraPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec3 unused = cameraPos;
     gl_Position = (projection * view * model * vec4(position, 1.0f));
    TexCoords = texCoords;
    FragPos = vec3(model*vec4(position,1.0f));
    //Normal = normal;
    Normal = mat3(transpose(inverse(model))) * normal;
}
