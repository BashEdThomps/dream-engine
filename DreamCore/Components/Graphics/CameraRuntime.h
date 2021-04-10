#pragma once

#define GLM_FORCE_RADIANS
#ifdef WIN32
#define _USE_MATH_DEFINES // for C++
#endif

#include "Base/Runtime.h"
#include "Math/Transform.h"
#include "Entity/BoundingBox.h"
#include "Frustum.h"

#include <optional>

using std::optional;
using nlohmann::json;
using glm::mat4;

namespace octronic::dream
{
  class SceneRuntime;
  class EntityRuntime;
  class CameraDefinition;

  class CameraRuntime : public Runtime
  {

  public:
    CameraRuntime(SceneRuntime& parent, CameraDefinition& cDef);
    CameraRuntime(CameraRuntime&&) = default;
    CameraRuntime& operator=(CameraRuntime&&) = default;
    ~CameraRuntime();

    void update();

    bool loadFromDefinition() override;

    bool visibleInFrustum(const EntityRuntime&)const;
    bool visibleInFrustum(const BoundingBox&) const;
    bool visibleInFrustum(const BoundingBox& bb,const mat4& tx) const;
    bool containedInFrustum(const EntityRuntime&) const;
    bool containedInFrustum(const BoundingBox&) const;
    bool containedInFrustumAfterTransform(const EntityRuntime&,const mat4& tx) const;
    bool exceedsFrustumPlaneAtTranslation(Frustum::Plane plane, const EntityRuntime& sor, const vec3& tx) const;

    void setTransform(const Transform&);
    Transform getTransform() const;

    mat4 getViewMatrix() const;
    mat4 getProjectionMatrix() const;
    void setProjectionMatrix(const mat4& projectionMatrix);

    void updateProjectionMatrix(float w, float h);

    void setFieldOfViewDegrees(float fov);
    float getFieldOfViewDegrees() const;

    UuidType getCameraEntityUuid() const;
    void setCameraEntityUuid(UuidType u);

    EntityRuntime& getCameraEntityRuntime();
    void setCameraEntityRuntime(EntityRuntime& er);

    void setMeshCullDistance(float);
    float getMeshCullDistance() const;

    float getMinDrawDistance() const;
    void setMinDrawDistance(float);

    float getMaxDrawDistance() const;
    void setMaxDrawDistance(float);

    void setUseEntity(bool ue);
    bool getUseEntity() const;

    void captureDefinition();
    SceneRuntime& getSceneRuntime() const;

  private:
    Transform mFreeTransform;
    UuidType mCameraEntityUuid;
    optional<reference_wrapper<EntityRuntime>> mCameraEntityRuntime;
    mat4 mProjectionMatrix;
    Frustum mFrustum;
    reference_wrapper<SceneRuntime> mSceneRuntime;
    bool mUseEntity;
    float mFieldOfViewDegrees;
    float mMinDrawDistance;
    float mMaxDrawDistance;
    float mMeshCullDistance;
  };
}
