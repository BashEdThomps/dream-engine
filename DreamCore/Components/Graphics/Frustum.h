#pragma once


#include "Entity/BoundingBox.h"

#include <glm/matrix.hpp>

using glm::mat4;
using std::reference_wrapper;

namespace octronic::dream
{
    class CameraRuntime;
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

        Frustum(CameraRuntime& cam);
        ~Frustum();
        void updatePlanes();
        Frustum::TestResult testIntersection(const mat4& modelMatrix, const BoundingBox& box) const;
        Frustum::TestResult testIntersectionWithPlane(Plane plane, const vec3& modelPos, const BoundingBox& box) const;

    protected:
        vec4 mPlanes[6];
        reference_wrapper<CameraRuntime> mCamera;
    };
}
