#pragma once

#include <glm/glm.hpp>

using glm::vec2;
using glm::vec3;


namespace octronic::dream
{
  struct __attribute__ ((packed)) Vertex
  {
    vec3 Position;
    vec2 TexCoords;
    vec3 Normal;
  };
}
