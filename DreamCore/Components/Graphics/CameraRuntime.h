/*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#pragma once

#define GLM_FORCE_RADIANS

#ifdef WIN32
    #define _USE_MATH_DEFINES // for C++
#endif

#include "Base/Runtime.h"
#include "Math/Transform.h"
#include "Entity/BoundingBox.h"
#include "Frustum.h"

using nlohmann::json;
using glm::mat4;

namespace octronic::dream
{
    class SceneRuntime;
    class EntityRuntime;
	class CameraDefinition;

    /**
     * @brief Implements a Camera in 3D Space.
     */
    class CameraRuntime : public Runtime
    {

    public:
        CameraRuntime(const weak_ptr<CameraDefinition>& cDef,
                      const weak_ptr<SceneRuntime>& parent);
        ~CameraRuntime();

        void update();

		bool loadFromDefinition() override;

        bool visibleInFrustum(const weak_ptr<EntityRuntime>&)const;
        bool visibleInFrustum(const BoundingBox&) const;
        bool visibleInFrustum(const BoundingBox& bb,const mat4& tx) const;
        bool containedInFrustum(const weak_ptr<EntityRuntime>&) const;
        bool containedInFrustum(const BoundingBox&) const;
        bool containedInFrustumAfterTransform(const weak_ptr<EntityRuntime>&,const mat4& tx) const;
        bool exceedsFrustumPlaneAtTranslation(Frustum::Plane plane, const weak_ptr<EntityRuntime>&sor, const vec3& tx) const;

        void setTransform(const Transform&);
        Transform getTransform() const;

        mat4 getViewMatrix() const;
        mat4 getProjectionMatrix() const;
        void setProjectionMatrix(const mat4& projectionMatrix);

        void updateProjectionMatrix(float w, float h);

        void setFieldOfView(float fov);
        float getFieldOfView() const;

        UuidType getCameraEntityUuid() const;
        void setCameraEntityUuid(UuidType u);

        weak_ptr<EntityRuntime> getCameraEntityRuntime() const;
        void setCameraEntityRuntime(const weak_ptr<EntityRuntime>& er);

        void setMeshCullDistance(float);
        float getMeshCullDistance() const;

        float getMinDrawDistance() const;
        void setMinDrawDistance(float);

        float getMaxDrawDistance() const;
        void setMaxDrawDistance(float);

        void setUseEntity(bool ue);
        bool getUseEntity() const;

		void captureDefinition();

    private:
        Transform mFreeTransform;
        UuidType mCameraEntityUuid;
        weak_ptr<EntityRuntime> mCameraEntityRuntime;
        mat4 mProjectionMatrix;
        Frustum mFrustum;
        weak_ptr<SceneRuntime> mSceneRuntime;
        bool mUseEntity;
        float mFieldOfView;
        float mMinDrawDistance;
        float mMaxDrawDistance;
        float mMeshCullDistance;
    };
}
