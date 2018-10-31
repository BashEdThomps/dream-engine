#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec3 Tangent;
out vec3 Bitangent;
out mat3 TBN;
out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;

uniform mat4 model[100];
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model[gl_InstanceID] * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model[gl_InstanceID]))) * aNormal;  
    TexCoords = aTexCoords;
    Tangent = aTangent;
    Bitangent = aBitangent;
    vec3 T = normalize(vec3(model[gl_InstanceID] * vec4(aTangent,   0.0)));
    vec3 B = normalize(vec3(model[gl_InstanceID] * vec4(aBitangent, 0.0)));
    vec3 N = normalize(vec3(model[gl_InstanceID] * vec4(aNormal,    0.0)));
    TBN = mat3(T, B, N);
    gl_Position = projection * view * vec4(FragPos, 1.0);
}

