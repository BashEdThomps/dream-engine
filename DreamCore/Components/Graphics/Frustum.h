#pragma once

#include "Common/Math.h"
#include "Scene/Entity/BoundingBox.h"

#include <glm/matrix.hpp>

using glm::mat4;

namespace Dream
{
    class Camera;
    class Frustum
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
        Frustum::TestResult testIntersectionWithPlane(Plane plane, const Vector3& modelPos, const BoundingBox& box) const;

    protected:
        Vector4 mPlanes[6];
        Camera* mCamera;
    };
}
