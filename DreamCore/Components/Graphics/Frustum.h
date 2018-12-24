#pragma once

#include "../../Common/DreamObject.h"
#include <glm/matrix.hpp>
#include "../../Scene/SceneObject/BoundingBox.h"

using namespace glm;

namespace Dream
{
    class Camera;
    class Frustum : public DreamObject
    {
    public:
        enum Plane
        {
            PLANE_BACK,
            PLANE_FRONT,
            PLANE_RIGHT,
            PLANE_LEFT,
            PLANE_TOP,
            PLANE_BOTTOM
        };

        enum TestResult
        {
            TEST_OUTSIDE,
            TEST_INTERSECT,
            TEST_INSIDE
        };

        Frustum(Camera* cam);
        ~Frustum();
        void updatePlanes();
        Frustum::TestResult testIntersection(const mat4& modelMatrix, const BoundingBox& box) const;
        Frustum::TestResult testIntersectionWithPlane(Plane plane, const vec3& modelMatrix, const BoundingBox& box) const;

    protected:
        vec4 mPlanes[6];
        Camera* mCamera;
    };
}
