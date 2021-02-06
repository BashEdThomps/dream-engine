#version 300 es

in vec2 texpos;
out vec4 FragColor;

uniform sampler2D tex;

void main(void) 
{
  FragColor = texture(tex, texpos);
}
