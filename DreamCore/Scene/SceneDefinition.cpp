/*
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */

#include "SceneDefinition.h"
#include "SceneDefinition.h"
#include "Common/Logger.h"
#include "Common/Constants.h"
#include "Common/Uuid.h"
#include "Math/Transform.h"
#include "Project/ProjectDefinition.h"
#include "Math/Vector.h"

namespace octronic::dream
{
  SceneDefinition::SceneDefinition
  (ProjectDefinition& projectDefinition, const json& data)
    : Definition(data),
      mCameraDefinition(json::object()),
      mProjectDefinition(projectDefinition)
  {
    LOG_TRACE( "SceneDefinition: Constructing {}", getNameAndUuidString() );

    if (mJson.find(Constants::SCENE_CAMERA) != mJson.end())
    {
    	mCameraDefinition.setJson(mJson[Constants::SCENE_CAMERA]);
    }

    if (mJson.find(Constants::SCENE_ROOT_SCENE_ENTITY) != mJson.end())
    {
      mRootSceneEntityDefinition.emplace(
      	*this,
      	optional<reference_wrapper<SceneEntityDefinition>>(),
      	mJson[Constants::SCENE_ROOT_SCENE_ENTITY]);
    }
  }

  // Rendering ===============================================================

  vec4
  SceneDefinition::getClearColor
  ()
  const
  {
    if (mJson.find(Constants::SCENE_CLEAR_COLOR) == mJson.end())
    {
      return vec4(0.f);
    }
    return Vector4::fromJson(mJson[Constants::SCENE_CLEAR_COLOR]);
  }

  void
  SceneDefinition::setClearColor
  (const vec4& color)
  {
    mJson[Constants::SCENE_CLEAR_COLOR] = Vector3::toJson(color);
  }

  UuidType
  SceneDefinition::getShadowPassShader
  ()
  const
  {
    if (mJson.find(Constants::SCENE_SHADOW_PASS_SHADER) == mJson.end())
    {
      return Uuid::INVALID;
    }
    return  mJson[Constants::SCENE_SHADOW_PASS_SHADER];
  }

  void
  SceneDefinition::setShadowPassShader
  (UuidType shader)
  {
    mJson[Constants::SCENE_SHADOW_PASS_SHADER] = shader;
  }

  UuidType
  SceneDefinition::getFontShader
  ()
  const
  {
    if (mJson.find(Constants::SCENE_FONT_SHADER) == mJson.end())
    {
      return Uuid::INVALID;
    }
    return  mJson[Constants::SCENE_FONT_SHADER];
  }

  void
  SceneDefinition::setFontShader
  (UuidType shader)
  {
    mJson[Constants::SCENE_FONT_SHADER] = shader;
  }

  UuidType
  SceneDefinition::getSpriteShader
  ()
  const
  {
    if (mJson.find(Constants::SCENE_SPRITE_SHADER) == mJson.end())
    {
      return Uuid::INVALID;
    }
    return  mJson[Constants::SCENE_SPRITE_SHADER];
  }

  void
  SceneDefinition::setSpriteShader
  (UuidType shader)
  {
    mJson[Constants::SCENE_SPRITE_SHADER] = shader;
  }

  void
  SceneDefinition::setCameraDefinitionFromJson
  (const json& cDef)
  {
    mCameraDefinition.setJson(cDef);
  }

  CameraDefinition&
  SceneDefinition::getCameraDefinition
  ()
  {
    return mCameraDefinition;
  }

  UuidType
  SceneDefinition::getEnvironmentTexture
  ()
  const
  {
    if (mJson.find(Constants::SCENE_ENVIRONMENT_TEXTURE) == mJson.end())
    {
      return Uuid::INVALID;
    }
    return mJson[Constants::SCENE_ENVIRONMENT_TEXTURE];
  }

  void
  SceneDefinition::setEnvironmentTexture
  (UuidType u)
  {
    mJson[Constants::SCENE_ENVIRONMENT_TEXTURE] = u;
  }

  UuidType
  SceneDefinition::getEnvironmentShader
  ()
  const
  {
    if (mJson.find(Constants::SCENE_ENVIRONMENT_SHADER) == mJson.end())
    {
      return Uuid::INVALID;
    }
    return mJson[Constants::SCENE_ENVIRONMENT_SHADER];
  }

  void
  SceneDefinition::setEnvironmentShader
  (UuidType u)
  {
    mJson[Constants::SCENE_ENVIRONMENT_SHADER] = u;
  }

  // Input ===================================================================

  UuidType
  SceneDefinition::getInputScript
  ()
  const
  {
    if (mJson.find(Constants::SCENE_INPUT_SCRIPT) == mJson.end())
    {
      return Uuid::INVALID;
    }
    return  mJson[Constants::SCENE_INPUT_SCRIPT];
  }

  void
  SceneDefinition::setInputScript
  (UuidType shader)
  {
    mJson[Constants::SCENE_INPUT_SCRIPT] = shader;
  }

  // Physics =================================================================

  vec3
  SceneDefinition::getGravity
  ()
  const
  {
    if (mJson.find(Constants::SCENE_GRAVITY) == mJson.end())
    {
      return vec3(0.f);
    }

    return Vector3::fromJson(mJson[Constants::SCENE_GRAVITY]);
  }

  void
  SceneDefinition::setGravity
  (const vec3& gravity)
  {
    mJson[Constants::SCENE_GRAVITY] = Vector3::toJson(gravity);
  }

  // Entity Management =======================================================



  ProjectDefinition&
  SceneDefinition::getProjectDefinition
  ()
  {
    return mProjectDefinition;
  }

  optional<SceneEntityDefinition>&
  SceneDefinition::getRootSceneEntityDefinition
  ()
  {
    return mRootSceneEntityDefinition;
  }


  optional<reference_wrapper<SceneEntityDefinition>>
  SceneDefinition::getSceneEntityByUuid(UuidType uuid)
  {
   if (mRootSceneEntityDefinition)
   {
     auto& root = mRootSceneEntityDefinition.value();

     if (root.hasUuid(uuid)) return root;

     auto child_result = root.getChildDefinitionByUuid(uuid);

     if (child_result) return child_result;
   }
   return std::nullopt;
  }

  // Serialisation ===========================================================

  json
  SceneDefinition::toJson
  ()
  {
    mJson[Constants::SCENE_ROOT_SCENE_ENTITY] = mRootSceneEntityDefinition.value().toJson();
    mJson[Constants::SCENE_CAMERA] = mCameraDefinition.toJson();
    return mJson;
  }
}
