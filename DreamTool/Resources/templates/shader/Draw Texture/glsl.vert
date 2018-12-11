#version 330 core

// Draw - Textured

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 TangentViewPos;
out vec3 TangentFragPos;
out vec3 Normal;
out mat3 TBN;

uniform mat4 model[100];
uniform mat4 view;
uniform mat4 projection;
uniform vec3 viewPos;

void main()
{
    gl_Position = projection * view * model[gl_InstanceID] * vec4(aPos, 1.0);
    FragPos = vec3(model[gl_InstanceID] * vec4(aPos, 1.0));
    TexCoords = aTexCoords;
    Normal = normalize(mat3(model[gl_InstanceID]) * aNormal);

    vec3 T   = normalize(mat3(model[gl_InstanceID]) * aTangent);
    vec3 B   = normalize(mat3(model[gl_InstanceID]) * aBitangent);
    vec3 N   = normalize(mat3(model[gl_InstanceID]) * aNormal);
    TBN = transpose(mat3(T, B, N));

    TangentViewPos  = TBN * viewPos;
    TangentFragPos  = TBN * FragPos;
}

