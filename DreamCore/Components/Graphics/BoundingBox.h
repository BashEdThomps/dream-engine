#pragma once
#define GLM_FORCE_RADIANS
#include <glm/vec3.hpp>
#include "../../Common/DreamObject.h"

using namespace glm;
class aiMesh;

namespace Dream
{
    class BoundingBox : DreamObject
    {
    public:
        BoundingBox(glm::vec3 minimum = {-1.0f,-1.0f,-1.0f}, vec3 maximum = {1.0f,1.0f,1.0f});
        ~BoundingBox();

        float maxDimension;
        void init();
        vec3 minimum;
        vec3 maximum;
        vec3 getCenter();
        void updateFromMesh(aiMesh*);
    };
}


