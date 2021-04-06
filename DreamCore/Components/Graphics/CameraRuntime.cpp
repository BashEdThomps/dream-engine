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

#include "CameraRuntime.h"
#include "CameraDefinition.h"

#include "Common/Logger.h"
#include "Common/Constants.h"
#include "Components/Window/WindowComponent.h"
#include "Project/ProjectRuntime.h"
#include "Scene/SceneRuntime.h"
#include "Entity/EntityRuntime.h"

#include <glm/common.hpp>

using glm::perspective;
using std::static_pointer_cast;

namespace octronic::dream
{
  CameraRuntime::CameraRuntime
  (SceneRuntime& parent, CameraDefinition& cDef)
    : Runtime(cDef),
      mFreeTransform(true),
      mCameraEntityUuid(Uuid::INVALID),
      mProjectionMatrix(mat4(1.f)),
      mFrustum(*this),
      mSceneRuntime(parent),
      mUseEntity(false),
      mFieldOfView(90.f),
      mMinDrawDistance(0.1f),
      mMaxDrawDistance(1000.f),
      mMeshCullDistance(100.f)
  {
  }

  CameraRuntime::~CameraRuntime
  ()
  {
    LOG_DEBUG("Camera: Destroying Object");
  }

  bool CameraRuntime::loadFromDefinition()
  {
    auto& cDef = static_cast<CameraDefinition&>(getDefinition());
    mFreeTransform = cDef.getFreeTransform();
    mCameraEntityUuid = cDef.getCameraEntityUuid();
    mUseEntity = cDef.getUseEntity();
    mFieldOfView = cDef.getFieldOfView();
    mMinDrawDistance = cDef.getMinDrawDistance();
    mMaxDrawDistance = cDef.getMaxDrawDistance();
    mMeshCullDistance = cDef.getMeshCullDistance();
    return true;
  }

  mat4
  CameraRuntime::getViewMatrix
  ()
  const
  {
    if (mUseEntity)
    {
      if(mCameraEntityRuntime)
      {
        return mCameraEntityRuntime.value().get().getTransform().getMatrix();
      }
      else
      {
        return mat4(1.f);
      }
    }
    else
    {
      return mFreeTransform.getMatrix();
    }
  }

  void
  CameraRuntime::update
  ()
  {
    if (mUseEntity)
    {
      mCameraEntityRuntime.emplace(reference_wrapper<EntityRuntime>(getSceneRuntime().getEntityRuntimeByUuid(mCameraEntityUuid)));
    }

    auto& projectRuntime = getSceneRuntime().getProjectRuntime();
    auto& windowComponent = projectRuntime.getWindowComponent();

    float windowWidth  = (float)windowComponent.getWidth();
    float windowHeight = (float)windowComponent.getHeight();
    if (windowWidth == 0.f || windowHeight == 0.f) return;

    mProjectionMatrix = perspective(mFieldOfView, windowWidth/windowHeight,
                                    mMinDrawDistance, mMaxDrawDistance);
    mFrustum.updatePlanes();
  }

  bool
  CameraRuntime::containedInFrustum
  (const EntityRuntime& er)
  const
  {
    return mFrustum.testIntersection(er.getTransform().getMatrix(),
                                     er.getBoundingBox()) == Frustum::TEST_INSIDE;
    return false;
  }

  bool
  CameraRuntime::containedInFrustum
  (const BoundingBox& bb)
  const
  {
    const static mat4 tx(1.0f);
    return mFrustum.testIntersection(tx,bb) == Frustum::TEST_INSIDE;
  }

  bool
  CameraRuntime::exceedsFrustumPlaneAtTranslation
  (Frustum::Plane plane, const EntityRuntime& er, const vec3& tx)
  const
  {
    auto result = mFrustum.testIntersectionWithPlane(plane, tx, er.getBoundingBox());
    return result != Frustum::TEST_INSIDE;
  }

  bool
  CameraRuntime::containedInFrustumAfterTransform
  (const EntityRuntime& er, const mat4& tx)
  const
  {
    return mFrustum.testIntersection(er.getTransform().getMatrix() * tx,
                                     er.getBoundingBox()) != Frustum::TEST_OUTSIDE;
    return false;
  }

  bool
  CameraRuntime::visibleInFrustum
  (const EntityRuntime& er)
  const
  {
    return mFrustum.testIntersection(er.getTransform().getMatrix(),
                                     er.getBoundingBox()) != Frustum::TEST_OUTSIDE;
  }

  bool
  CameraRuntime::visibleInFrustum
  (const BoundingBox& bb)
  const
  {
    const static mat4 tx(1.0f);
    return mFrustum.testIntersection(tx,bb) != Frustum::TEST_OUTSIDE;
  }

  bool
  CameraRuntime::visibleInFrustum
  (const BoundingBox& bb, const mat4& tx)
  const
  {
    return mFrustum.testIntersection(tx,bb) != Frustum::TEST_OUTSIDE;
  }

  mat4
  CameraRuntime::getProjectionMatrix
  () const
  {
    return mProjectionMatrix;
  }

  void
  CameraRuntime::setProjectionMatrix
  (const mat4& projectionMatrix)
  {
    mProjectionMatrix = projectionMatrix;
  }

  void
  CameraRuntime::setFieldOfView
  (float fov)
  {
    mFieldOfView = fov;
  }

  float
  CameraRuntime::getFieldOfView
  () const
  {
    return mFieldOfView;
  }

  void
  CameraRuntime::setCameraEntityRuntime
  (EntityRuntime& er)
  {
    mCameraEntityRuntime.emplace(reference_wrapper<EntityRuntime>(er));
  }

  EntityRuntime&
  CameraRuntime::getCameraEntityRuntime
  ()
  {
    return mCameraEntityRuntime.value().get();
  }

  UuidType
  CameraRuntime::getCameraEntityUuid
  () const
  {
    return mCameraEntityUuid;
  }

  void
  CameraRuntime::setCameraEntityUuid
  (UuidType u)
  {
    mCameraEntityUuid = u;
  }

  void
  CameraRuntime::setTransform
  (const Transform& t)
  {
    if (mUseEntity)
    {
      if (mCameraEntityRuntime)
      {
        mCameraEntityRuntime.value().get().setTransform(t);
      }
    }
    else
    {
      mFreeTransform = t;
    }
  }

  Transform CameraRuntime::getTransform() const
  {
    if (mUseEntity)
    {
      if (mCameraEntityRuntime)
      {
        return mCameraEntityRuntime.value().get().getTransform();
      }
      else
      {
        return Transform();
      }
    }
    else
    {
      return mFreeTransform;
    }
  }

  void
  CameraRuntime::setMeshCullDistance
  (float mcd)
  {
    mMeshCullDistance = mcd;
  }

  float
  CameraRuntime::getMeshCullDistance
  ()
  const
  {
    return mMeshCullDistance;
  }

  void
  CameraRuntime::setMinDrawDistance
  (float f)
  {
    mMinDrawDistance = f;
  }

  float
  CameraRuntime::getMinDrawDistance
  ()
  const
  {
    return mMinDrawDistance;
  }

  float
  CameraRuntime::getMaxDrawDistance
  ()
  const
  {
    return mMaxDrawDistance;
  }

  void
  CameraRuntime::setMaxDrawDistance
  (float mdd)
  {
    mMaxDrawDistance = mdd;
  }

  void
  CameraRuntime::setUseEntity
  (bool ue)
  {
    mUseEntity = ue;
  }

  bool
  CameraRuntime::getUseEntity
  () const
  {
    return mUseEntity;
  }

  void
  CameraRuntime::captureDefinition
  ()
  {
    auto& cDef = static_cast<CameraDefinition&>(getDefinition());
    cDef.setFreeTransform(mFreeTransform);
    cDef.setCameraEntityUuid(mCameraEntityUuid);
    cDef.setUseEntity(mUseEntity);
    cDef.setFieldOfView(mFieldOfView);
    cDef.setMinDrawDistance(mMinDrawDistance);
    cDef.setMaxDrawDistance(mMaxDrawDistance);
    cDef.setMeshCullDistance(mMeshCullDistance);
  }

  SceneRuntime&
  CameraRuntime::getSceneRuntime() const
  {
    return mSceneRuntime.get();
  }
}
