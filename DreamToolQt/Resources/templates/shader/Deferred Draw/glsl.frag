#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    float shininess;
    vec3 diffuseColor;
    vec3 specularColor;
};

Material material;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

void main()
{
    // store the fragment position vector in the first gbuffer texture
    gPosition = FragPos;
    // also store the per-fragment normals into the gbuffer
    vec3 norm = texture(material.normal, TexCoords).rgb;
    norm = normalize(norm * 2.0 - 1.0);
    gNormal = normalize(TBN * norm);
    // and the diffuse per-fragment color
    gAlbedoSpec.rgb = texture(material.diffuse, TexCoords).rgb;
    // store specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a = texture(material.specular, TexCoords).r;
}
