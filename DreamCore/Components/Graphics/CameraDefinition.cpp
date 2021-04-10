#include "CameraDefinition.h"
#include "Common/Constants.h"

namespace octronic::dream
{
  CameraDefinition::CameraDefinition(const json& js)
    : Definition(js)
  {
    if (mJson.find(Constants::UUID) == mJson.end())
    {
      setUuid(Uuid::RandomUuid());
    }
  }

  float
  CameraDefinition::getFieldOfViewDegrees
  ()
  const
  {
    if (mJson.find(Constants::CAMERA_FOV_DEGREES) == mJson.end())
    {
      return 90.f;
    }
    return mJson[Constants::CAMERA_FOV_DEGREES];
  }

  void
  CameraDefinition::setFieldOfViewDegrees
  (float v)
  {
    mJson[Constants::CAMERA_FOV_DEGREES] = v;
  }


  UuidType
  CameraDefinition::getCameraEntityUuid
  ()
  const
  {
    if (mJson.find(Constants::CAMERA_ENTITY_UUID) == mJson.end())
    {
      return Uuid::INVALID;
    }
    return mJson[Constants::CAMERA_ENTITY_UUID];
  }

  void
  CameraDefinition::setCameraEntityUuid
  (UuidType u)
  {
    mJson[Constants::CAMERA_ENTITY_UUID] = u;
  }

  void
  CameraDefinition::setFreeTransform
  (const Transform& t)
  {
    mJson[Constants::CAMERA_FREE_TRANSFORM] = t.toJson();
  }

  Transform
  CameraDefinition::getFreeTransform
  ()
  const
  {
    if (mJson.find(Constants::CAMERA_FREE_TRANSFORM) == mJson.end())
    {
      return Transform();
    }
    return Transform(mJson[Constants::CAMERA_FREE_TRANSFORM]);
  }

  void
  CameraDefinition::setMeshCullDistance
  (float mcd)
  {
    mJson[Constants::CAMERA_MESH_CULL_DISTANCE] = mcd;
  }

  float
  CameraDefinition::getMeshCullDistance
  ()
  const
  {
    if (mJson.find(Constants::CAMERA_MESH_CULL_DISTANCE) == mJson.end())
    {
      return 100.f;
    }
    return mJson[Constants::CAMERA_MESH_CULL_DISTANCE];
  }

  void
  CameraDefinition::setMinDrawDistance
  (float f)
  {
    mJson[Constants::CAMERA_MIN_DRAW_DISTANCE] = f;
  }

  float
  CameraDefinition::getMinDrawDistance
  ()
  const
  {
    if (mJson.find(Constants::CAMERA_MIN_DRAW_DISTANCE) == mJson.end())
    {
      return .1f;
    }
    return mJson[Constants::CAMERA_MIN_DRAW_DISTANCE];
  }

  float
  CameraDefinition::getMaxDrawDistance
  ()
  const
  {
    if (mJson.find(Constants::CAMERA_MAX_DRAW_DISTANCE) == mJson.end())
    {
      return 100.f;
    }
    return mJson[Constants::CAMERA_MAX_DRAW_DISTANCE];
  }

  void
  CameraDefinition::setMaxDrawDistance
  (float mdd)
  {
    mJson[Constants::CAMERA_MAX_DRAW_DISTANCE] = mdd;
  }

  void
  CameraDefinition::setUseEntity
  (bool ue)
  {
    mJson[Constants::CAMERA_USE_ENTITY] = ue;
  }

  bool
  CameraDefinition::getUseEntity
  ()
  const
  {
    if (mJson.find(Constants::CAMERA_USE_ENTITY) == mJson.end())
    {
      return false;
    }
    return mJson[Constants::CAMERA_USE_ENTITY];
  }
}
