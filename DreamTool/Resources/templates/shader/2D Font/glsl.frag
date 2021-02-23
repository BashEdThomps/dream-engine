/* 
* Dream
* Shader Template: Font
* GLSL330 Core
*/
#version 330 core

in vec2 texturePos;
out vec4 FragColor;

uniform sampler2D uTexture;
uniform vec4 uColor;

void main(void) 
{
  FragColor =vec4(1, 1, 1, texture(uTexture, texturePos).r) * uColor;
}
