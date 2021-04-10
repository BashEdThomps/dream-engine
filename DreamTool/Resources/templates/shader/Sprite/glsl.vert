/* 
* Dream
* Shader Template: Sprite
*
* GLSL 330 Core
*/
#version 330 core

layout (location = 0) in vec4 coord;
out vec2 texpos;

uniform mat4 uModel;
uniform mat4 uProj;

void main(void) 
{
  gl_Position = uProj * uModel * vec4(coord.xy, 0, 1);
  texpos = coord.zw;
}
