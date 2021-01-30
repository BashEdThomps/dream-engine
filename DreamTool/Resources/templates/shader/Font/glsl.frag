#version 330 core

in vec2 texpos;
out vec4 FragColor;

uniform sampler2D tex;
uniform vec4 color;

void main(void) 
{
  FragColor =vec4(1, 1, 1, texture(tex, texpos).r) * color;
}
