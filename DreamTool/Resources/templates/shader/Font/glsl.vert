/* 
* Dream
* Shader Template: Font
* GLSL 330 Core
*/
#version 330 core

layout (location = 0) in vec4 Position;

out vec2 TexturePosition;

uniform mat4 uModelMatrix;
uniform mat4 uProjectionMatrix;

void main(void) 
{
  gl_Position = uProjectionMatrix * uModelMatrix * vec4(Position.xy, 0, 1);
  TexturePosition = Position.zw;
}
