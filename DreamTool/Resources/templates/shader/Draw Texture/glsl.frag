#version 330 core

// Draw - Textured

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out vec3 gIgnore;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    sampler2D displacement;
    float shininess;
    vec3 diffuseColor;
    vec3 specularColor;
    float ignore;
};

uniform Material material;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in mat3 TBN;

void main()
{
    gPosition = FragPos;
    gNormal = Normal;
    gAlbedoSpec.rgb = texture(material.diffuse, TexCoords).rgb;
    gAlbedoSpec.a = material.shininess;
    gIgnore.rgb = vec3(material.ignore);
}

