#version 300 es
// Draw - NonTextured

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out vec3 gDepthOut;
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    sampler2D displacement;
    float shininess;
    vec3 diffuseColor;
    vec3 specularColor;
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
    gDepthOut.rgb = gl_FragCoord.z;
    gAlbedoSpec.rgb = material.diffuseColor;
    gAlbedoSpec.a =  material.shininess;
}

