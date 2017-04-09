#version 330 core

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform sampler2D texture_diffuse1;
uniform vec3  ambientColor;
uniform float ambientStrength;
uniform vec3 lightPos;
uniform vec3 lightColor;

void main() {
    vec4 objectColor = vec4(texture(texture_diffuse1, TexCoords));
    // Calculate Ambient
    vec3 ambient = vec3(ambientStrength * ambientColor);
    // Calculate Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos-FragPos);
    float diff = max(dot(norm,lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    // Combine Ambient & diffuse
    vec4 ambientDiffuseColor = vec4((ambient + diffuse),1.0f) * objectColor;
    color = ambientDiffuseColor;
}
