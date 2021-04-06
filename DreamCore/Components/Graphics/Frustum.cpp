#include "Frustum.h"
#include "CameraRuntime.h"

namespace octronic::dream
{
    Frustum::Frustum(CameraRuntime& cam)
        : mCamera(cam)
    {

    }

    Frustum::~Frustum()
    {

    }

    void
    Frustum::updatePlanes
    ()
    {
        const mat4 &v = mCamera.get().getViewMatrix();
        const mat4 &p = mCamera.get().getProjectionMatrix();

        mat4 clipMatrix;

        clipMatrix[0][0] = v[0][0 ]*p[0][0]+v[0][1]*p[1][0]+v[0][2]*p[2][0]+v[0][3]*p[3][0];
        clipMatrix[1][0] = v[0][0]*p[0][1]+v[0][1]*p[1][1]+v[0][2]*p[2][1]+v[0][3]*p[3][1];
        clipMatrix[2][0] = v[0][0]*p[0][2]+v[0][1]*p[1][2]+v[0][2]*p[2][2]+v[0][3]*p[3][2];
        clipMatrix[3][0] = v[0][0]*p[0][3]+v[0][1]*p[1][3]+v[0][2]*p[2][3]+v[0][3]*p[3][3];
        clipMatrix[0][1] = v[1][0]*p[0][0]+v[1][1]*p[1][0]+v[1][2]*p[2][0]+v[1][3]*p[3][0];
        clipMatrix[1][1] = v[1][0]*p[0][1]+v[1][1]*p[1][1]+v[1][2]*p[2][1]+v[1][3]*p[3][1];
        clipMatrix[2][1] = v[1][0]*p[0][2]+v[1][1]*p[1][2]+v[1][2]*p[2][2]+v[1][3]*p[3][2];
        clipMatrix[3][1] = v[1][0]*p[0][3]+v[1][1]*p[1][3]+v[1][2]*p[2][3]+v[1][3]*p[3][3];
        clipMatrix[0][2] = v[2][0]*p[0][0]+v[2][1]*p[1][0]+v[2][2]*p[2][0]+v[2][3]*p[3][0];
        clipMatrix[1][2] = v[2][0]*p[0][1]+v[2][1]*p[1][1]+v[2][2]*p[2][1]+v[2][3]*p[3][1];
        clipMatrix[2][2] = v[2][0]*p[0][2]+v[2][1]*p[1][2]+v[2][2]*p[2][2]+v[2][3]*p[3][2];
        clipMatrix[3][2] = v[2][0]*p[0][3]+v[2][1]*p[1][3]+v[2][2]*p[2][3]+v[2][3]*p[3][3];
        clipMatrix[0][3] = v[3][0]*p[0][0]+v[3][1]*p[1][0]+v[3][2]*p[2][0]+v[3][3]*p[3][0];
        clipMatrix[1][3] = v[3][0]*p[0][1]+v[3][1]*p[1][1]+v[3][2]*p[2][1]+v[3][3]*p[3][1];
        clipMatrix[2][3] = v[3][0]*p[0][2]+v[3][1]*p[1][2]+v[3][2]*p[2][2]+v[3][3]*p[3][2];
        clipMatrix[3][3] = v[3][0]*p[0][3]+v[3][1]*p[1][3]+v[3][2]*p[2][3]+v[3][3]*p[3][3];

        mPlanes[PLANE_RIGHT].x = clipMatrix[3][0]-clipMatrix[0][0];
        mPlanes[PLANE_RIGHT].y = clipMatrix[3][1]-clipMatrix[0][1];
        mPlanes[PLANE_RIGHT].z = clipMatrix[3][2]-clipMatrix[0][2];
        mPlanes[PLANE_RIGHT].w = clipMatrix[3][3]-clipMatrix[0][3];

        mPlanes[PLANE_LEFT].x = clipMatrix[3][0]+clipMatrix[0][0];
        mPlanes[PLANE_LEFT].y = clipMatrix[3][1]+clipMatrix[0][1];
        mPlanes[PLANE_LEFT].z = clipMatrix[3][2]+clipMatrix[0][2];
        mPlanes[PLANE_LEFT].w = clipMatrix[3][3]+clipMatrix[0][3];

        mPlanes[PLANE_BOTTOM].x = clipMatrix[3][0]+clipMatrix[1][0];
        mPlanes[PLANE_BOTTOM].y = clipMatrix[3][1]+clipMatrix[1][1];
        mPlanes[PLANE_BOTTOM].z = clipMatrix[3][2]+clipMatrix[1][2];
        mPlanes[PLANE_BOTTOM].w = clipMatrix[3][3]+clipMatrix[1][3];

        mPlanes[PLANE_TOP].x = clipMatrix[3][0]-clipMatrix[1][0];
        mPlanes[PLANE_TOP].y = clipMatrix[3][1]-clipMatrix[1][1];
        mPlanes[PLANE_TOP].z = clipMatrix[3][2]-clipMatrix[1][2];
        mPlanes[PLANE_TOP].w = clipMatrix[3][3]-clipMatrix[1][3];

        mPlanes[PLANE_BACK].x = clipMatrix[3][0]-clipMatrix[2][0];
        mPlanes[PLANE_BACK].y = clipMatrix[3][1]-clipMatrix[2][1];
        mPlanes[PLANE_BACK].z = clipMatrix[3][2]-clipMatrix[2][2];
        mPlanes[PLANE_BACK].w = clipMatrix[3][3]-clipMatrix[2][3];

        mPlanes[PLANE_FRONT].x = clipMatrix[3][0]+clipMatrix[2][0];
        mPlanes[PLANE_FRONT].y = clipMatrix[3][1]+clipMatrix[2][1];
        mPlanes[PLANE_FRONT].z = clipMatrix[3][2]+clipMatrix[2][2];
        mPlanes[PLANE_FRONT].w = clipMatrix[3][3]+clipMatrix[2][3];

        for( int i = 0; i < 6; i++ )
        {
            mPlanes[i] = glm::normalize(mPlanes[i]);
        }
    }

    // check whether an AABB intersects the frustum
    Frustum::TestResult
    Frustum::testIntersection
    (const mat4& modelMatrix, const BoundingBox& box)
    const
    {
        const vec3 modelPos(modelMatrix[3][0],modelMatrix[3][1],modelMatrix[3][2]);
        TestResult result = TEST_INSIDE;
        for( uint i = 0; i < 6; i++ )
        {
            const float pos = mPlanes[i].w;
            const vec3 normal = vec3(mPlanes[i].x,mPlanes[i].y,mPlanes[i].z);
            if(glm::dot(normal, box.getPositiveVertex(modelPos, normal))+pos < 0.0f)
            {
                return TEST_OUTSIDE;
            }
            if(glm::dot(normal, box.getNegativeVertex(modelPos, normal))+pos < 0.0f)
            {
                result = TEST_INTERSECT;
            }
        }
        return result;
    }

    // check whether an AABB intersects the frustum
    Frustum::TestResult
    Frustum::testIntersectionWithPlane
    (Plane plane, const vec3& modelPos, const BoundingBox& box)
    const
    {
        TestResult result = TEST_INSIDE;
        const float pos = mPlanes[plane].w;
        const vec3 normal = vec3(mPlanes[plane].x,mPlanes[plane].y,mPlanes[plane].z);
        if(glm::dot(normal, box.getPositiveVertex(modelPos, normal))+pos < 0.0f)
        {
            return TEST_OUTSIDE;
        }
        if(glm::dot(normal, box.getNegativeVertex(modelPos, normal))+pos < 0.0f)
        {
            result = TEST_INTERSECT;
        }
        return result;
    }
}
