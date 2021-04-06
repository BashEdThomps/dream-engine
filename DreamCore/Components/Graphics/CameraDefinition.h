#pragma once

#include <Base/Definition.h>

namespace octronic::dream
{
  class CameraDefinition : public Definition
  {
  public:
    CameraDefinition(const json& js);

    CameraDefinition(CameraDefinition&&) = default;
    CameraDefinition& operator=(CameraDefinition&&) = default;

    void setFieldOfView(float fov);
    float getFieldOfView() const;

    Transform getFreeTransform() const;
    void setFreeTransform(const Transform& t);

    UuidType getCameraEntityUuid() const;
    void setCameraEntityUuid(UuidType u);

    void setMeshCullDistance(float);
    float getMeshCullDistance() const;

    float getMinDrawDistance() const;
    void setMinDrawDistance(float);

    float getMaxDrawDistance() const;
    void setMaxDrawDistance(float);

    void setUseEntity(bool ue);
    bool getUseEntity() const;

  };
}
