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
#include "Entity/EntityDefinition.h"
#include "Common/Logger.h"
#include "Common/Constants.h"
#include "Common/Uuid.h"
#include "Math/Transform.h"
#include "Project/ProjectDefinition.h"
#include "Math/Vector.h"

namespace octronic::dream
{
    SceneDefinition::SceneDefinition
    (ProjectDefinition* projectDefinition, const json& data)
        : Definition("SceneDefinition",data),
          mRootEntityDefinition(nullptr),
          mProjectDefinition(projectDefinition)
    {
        LOG_TRACE( "SceneDefinition: Constructing {}", getNameAndUuidString() );
    }

    SceneDefinition::~SceneDefinition
    ()
    {
        LOG_TRACE( "SceneDefinition: Destructing {}", getNameAndUuidString() );
        if (mRootEntityDefinition != nullptr)
        {
            delete mRootEntityDefinition;
            mRootEntityDefinition = nullptr;
        }
    }

    void
    SceneDefinition::loadRootEntityDefinition
    ()
    {
        if (mJson.find(Constants::SCENE_ROOT_ENTITY) == mJson.end())
        {
            LOG_ERROR( "SceneDefinition: No root Entity found!!" );
            return;
        }
        json rsoJson = mJson[Constants::SCENE_ROOT_ENTITY];

        mRootEntityDefinition = new EntityDefinition(nullptr, this, rsoJson );
        mRootEntityDefinition->loadChildEntityDefinitions();
    }

    void
    SceneDefinition::setPhysicsDebug
    (bool debug)
    {
        mJson[Constants::SCENE_PHYSICS_DEBUG] = debug;
    }

    bool
    SceneDefinition::getPhysicsDebug
    ()
    {
        if (mJson.find(Constants::SCENE_PHYSICS_DEBUG) == mJson.end())
        {
            mJson[Constants::SCENE_PHYSICS_DEBUG] = false;
        }
        return mJson[Constants::SCENE_PHYSICS_DEBUG];
    }

    void SceneDefinition::setMeshCullDistance(float mcd)
    {
        mJson[Constants::SCENE_MESH_CULL_DISTANCE] = mcd;
    }

    float SceneDefinition::getMeshCullDistance()
    {
        if (mJson.find(Constants::SCENE_MESH_CULL_DISTANCE) == mJson.end())
        {
            mJson[Constants::SCENE_MESH_CULL_DISTANCE] = 1000.0f;
        }
        return mJson[Constants::SCENE_MESH_CULL_DISTANCE];
    }

    void
    SceneDefinition::addTemplate
    (EntityDefinition* _template)
    {
        mTemplates.push_back(_template);
    }

    EntityDefinition*
    SceneDefinition::getTemplateByUuid
    (UuidType uuid)
    {
        for (EntityDefinition* next : mTemplates)
        {
            if (next->getUuid() == uuid)
            {
                return next;
            }
        }
        return nullptr;
    }

    Transform
    SceneDefinition::getCameraTransform
    ()
    {
        if (mJson.find(Constants::SCENE_CAMERA_TRANSFORM) == mJson.end())
        {
            mJson[Constants::SCENE_CAMERA_TRANSFORM] = Transform().toJson();
        }
        return Transform(mJson[Constants::SCENE_CAMERA_TRANSFORM]);

    }

    void
    SceneDefinition::setCameraTransform
    (const Transform& transform)
    {
        mJson[Constants::SCENE_CAMERA_TRANSFORM] = transform.toJson();
    }

    vec3
    SceneDefinition::getGravity
    ()
    {
        vec3 gravity;

        if (mJson.find(Constants::SCENE_GRAVITY) == mJson.end())
        {
            mJson[Constants::SCENE_GRAVITY] = Vector3::toJson(vec3(0.0f));
        }

        gravity = Vector3::fromJson(mJson[Constants::SCENE_GRAVITY]);

        return gravity;
    }

    void
    SceneDefinition::setGravity
    (const vec3& gravity)
    {
        mJson[Constants::SCENE_GRAVITY] = Vector3::toJson(gravity);
    }

    vec4
    SceneDefinition::getClearColor
    ()
    {
        vec4 color;

        if (mJson.find(Constants::SCENE_CLEAR_COLOR) == mJson.end())
        {
            mJson[Constants::SCENE_CLEAR_COLOR] = Vector4::toJson(vec4(0.f));
        }

        color = Vector4::fromJson(mJson[Constants::SCENE_CLEAR_COLOR]);
        return color;

    }

    void
    SceneDefinition::setClearColor
    (const vec4& color)
    {
        mJson[Constants::SCENE_CLEAR_COLOR] = Vector3::toJson(color);
    }

    EntityDefinition*
    SceneDefinition::getRootEntityDefinition
    ()
    {
        return mRootEntityDefinition;
    }

    ProjectDefinition*
    SceneDefinition::getProjectDefinition
    ()
    {
        return mProjectDefinition;
    }

    EntityDefinition*
    SceneDefinition::createNewRootEntityDefinition
    ()
    {
        json rootDefJson;
        rootDefJson[Constants::NAME] = Constants::ENTITY_ROOT_NAME;
        rootDefJson[Constants::UUID] = Uuid::generateUuid();
        rootDefJson[Constants::TRANSFORM] = Transform().toJson();
        mRootEntityDefinition = new EntityDefinition(nullptr,this,rootDefJson);
        return mRootEntityDefinition;
    }

    json
    SceneDefinition::getJson
    ()
    {
        mJson[Constants::SCENE_ROOT_ENTITY] = mRootEntityDefinition->getJson();
        return mJson;
    }

    void SceneDefinition::setMinDrawDistance(float mdd)
    {
        mJson[Constants::SCENE_MIN_DRAW_DISTANCE] = mdd;
    }

    float SceneDefinition::getMinDrawDistance()
    {
        if (mJson.find(Constants::SCENE_MIN_DRAW_DISTANCE) == mJson.end())
        {
            mJson[Constants::SCENE_MIN_DRAW_DISTANCE] = 0.1f;
        }
        return mJson[Constants::SCENE_MIN_DRAW_DISTANCE];
    }

    void SceneDefinition::setMaxDrawDistance(float mdd)
    {
        mJson[Constants::SCENE_MAX_DRAW_DISTANCE] = mdd;

    }

    float SceneDefinition::getMaxDrawDistance()
    {
        if (mJson.find(Constants::SCENE_MAX_DRAW_DISTANCE) == mJson.end())
        {
            mJson[Constants::SCENE_MAX_DRAW_DISTANCE] = 1000.0f;
        }
        return mJson[Constants::SCENE_MAX_DRAW_DISTANCE];
    }

    UuidType
    SceneDefinition::getShadowPassShader
    ()
    {
        if (mJson.find(Constants::SCENE_SHADOW_PASS_SHADER) == mJson.end())
        {
            mJson[Constants::SCENE_SHADOW_PASS_SHADER] = 0;
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
    {
        if (mJson.find(Constants::SCENE_FONT_SHADER) == mJson.end())
        {
            mJson[Constants::SCENE_FONT_SHADER] = 0;
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
    {
        if (mJson.find(Constants::SCENE_SPRITE_SHADER) == mJson.end())
        {
            mJson[Constants::SCENE_SPRITE_SHADER] = 0;
        }
        return  mJson[Constants::SCENE_SPRITE_SHADER];
    }

    void
    SceneDefinition::setSpriteShader
    (UuidType shader)
    {
        mJson[Constants::SCENE_SPRITE_SHADER] = shader;
    }

    UuidType
    SceneDefinition::getInputScript
    ()
    {
        if (mJson.find(Constants::SCENE_INPUT_SCRIPT) == mJson.end())
        {
            mJson[Constants::SCENE_INPUT_SCRIPT] = 0;
        }
        return  mJson[Constants::SCENE_INPUT_SCRIPT];
    }

    void
    SceneDefinition::setInputScript
    (UuidType shader)
    {
        mJson[Constants::SCENE_INPUT_SCRIPT] = shader;
    }

    void
    SceneDefinition::setPlayerObject
    (UuidType po)
    {
        mJson[Constants::ENTITY_PLAYER_OBJECT] = po;
    }

    UuidType SceneDefinition::getPlayerObject()
    {
        if (mJson.find(Constants::ENTITY_PLAYER_OBJECT) == mJson.end())
        {
            mJson[Constants::ENTITY_PLAYER_OBJECT] = 0;
        }
        return mJson[Constants::ENTITY_PLAYER_OBJECT];
    }

	void SceneDefinition::setCameraFOV(float fov)
	{
        mJson[Constants::SCENE_CAMERA_FOV] = fov;
	}

	float SceneDefinition::getCameraFOV()
	{
        if (mJson.find(Constants::SCENE_CAMERA_FOV) == mJson.end())
        {
           mJson[Constants::SCENE_CAMERA_FOV] = 0.f;
        }
        return mJson[Constants::SCENE_CAMERA_FOV];
	}

	UuidType SceneDefinition::getEnvironmentTexture()
    {
       if (mJson.find(Constants::SCENE_ENVIRONMENT_TEXTURE) == mJson.end())
       {
           mJson[Constants::SCENE_ENVIRONMENT_TEXTURE] = 0;
       }
       return mJson[Constants::SCENE_ENVIRONMENT_TEXTURE];
    }

	void SceneDefinition::setEnvironmentTexture(UuidType u)
    {
       mJson[Constants::SCENE_ENVIRONMENT_TEXTURE] = u;
    }

    UuidType SceneDefinition::getEnvironmentShader()
    {
       if (mJson.find(Constants::SCENE_ENVIRONMENT_SHADER) == mJson.end())
       {
           mJson[Constants::SCENE_ENVIRONMENT_SHADER] = 0;
       }
       return mJson[Constants::SCENE_ENVIRONMENT_SHADER];
    }

	void SceneDefinition::setEnvironmentShader(UuidType u)
    {
       mJson[Constants::SCENE_ENVIRONMENT_SHADER] = u;
    }
}
