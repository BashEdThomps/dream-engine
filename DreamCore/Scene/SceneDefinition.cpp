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
#include "Components/Transform.h"
#include "Project/ProjectDefinition.h"



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
    SceneDefinition::setCameraMovementSpeed
    (float speed)
    {
        mJson[Constants::SCENE_CAMERA_MOVEMENT_SPEED] = speed;
    }

    float
    SceneDefinition::getCameraMovementSpeed
    ()
    {
        if (mJson.find(Constants::SCENE_CAMERA_MOVEMENT_SPEED) == mJson.end())
        {
            mJson[Constants::SCENE_CAMERA_MOVEMENT_SPEED] = Constants::SCENE_CAMERA_DEFAULT_MOVEMENT_SPEED;
        }
        return mJson[Constants::SCENE_CAMERA_MOVEMENT_SPEED];
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

    Vector3
    SceneDefinition::getCameraTranslation
    ()
    {
        if (mJson.find(Constants::SCENE_CAMERA_TRANSLATION) == mJson.end())
        {
            setCameraTranslation(Vector3(0.0f));
        }
        return Vector3(mJson[Constants::SCENE_CAMERA_TRANSLATION]);

    }

    void
    SceneDefinition::setCameraTranslation
    (const Vector3& transform)
    {
        // Translation
        mJson[Constants::SCENE_CAMERA_TRANSLATION] = transform.toJson();
    }

    Vector3 SceneDefinition::getCameraLookAt()
    {
        if (mJson.find(Constants::SCENE_CAMERA_LOOK_AT) == mJson.end())
        {
            setCameraLookAt(Vector3(0.0f));
        }
        return Vector3(mJson[Constants::SCENE_CAMERA_LOOK_AT]);
    }

    void
    SceneDefinition::setCameraLookAt
    (const Vector3& lookAt)
    {
        mJson[Constants::SCENE_CAMERA_LOOK_AT] = lookAt.toJson();
    }

    Vector3
    SceneDefinition::getGravity
    ()
    {
        Vector3 gravity;

        if (mJson.find(Constants::SCENE_GRAVITY) == mJson.end())
        {
            mJson[Constants::SCENE_GRAVITY] = Vector3(0.0f).toJson();
        }

        gravity = Vector3(mJson[Constants::SCENE_GRAVITY]);

        return gravity;
    }

    void
    SceneDefinition::setGravity
    (const Vector3& gravity)
    {
        mJson[Constants::SCENE_GRAVITY] = gravity.toJson();
    }

    void
    SceneDefinition::setGravityX
    (float x)
    {
        mJson[Constants::SCENE_GRAVITY][Constants::X] = x;
    }

    void
    SceneDefinition::setGravityY
    (float y)
    {
        mJson[Constants::SCENE_GRAVITY][Constants::Y] = y;
    }

    void
    SceneDefinition::setGravityZ
    (float z)
    {
        mJson[Constants::SCENE_GRAVITY][Constants::Z] = z;
    }

    Vector3
    SceneDefinition::getClearColour
    ()
    {
        Vector3 colour;

        if (mJson.find(Constants::SCENE_CLEAR_COLOUR) == mJson.end())
        {
            mJson[Constants::SCENE_CLEAR_COLOUR] = Vector3(0.f).toJson();
        }

        colour = Vector3(mJson[Constants::SCENE_CLEAR_COLOUR]);
        return colour;

    }

    void
    SceneDefinition::setClearColour
    (const Vector3& colour)
    {


        if (mJson.find(Constants::SCENE_CLEAR_COLOUR) == mJson.end())
        {
            mJson[Constants::SCENE_CLEAR_COLOUR] = Vector3(0.f).toJson();
        }

        mJson[Constants::SCENE_CLEAR_COLOUR] = colour.toJson();

    }

    void
    SceneDefinition::setClearColourR
    (float r)
    {


        mJson[Constants::SCENE_CLEAR_COLOUR][Constants::RED]  = r;

    }

    void
    SceneDefinition::setClearColourG
    (float g)
    {


        mJson[Constants::SCENE_CLEAR_COLOUR][Constants::GREEN]  = g;

    }

    void
    SceneDefinition::setClearColourB
    (float b)
    {
        mJson[Constants::SCENE_CLEAR_COLOUR][Constants::BLUE]  = b;
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
        Transform transform;
        rootDefJson[Constants::TRANSFORM] = transform.getJson();
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

    float SceneDefinition::getCameraTranslationX()
    {
        if (mJson.find(Constants::SCENE_CAMERA_TRANSLATION) == mJson.end())
        {
            setCameraTranslation(Vector3(0.0f));
        }
        return mJson[Constants::SCENE_CAMERA_TRANSLATION][Constants::X];
    }

    float SceneDefinition::getCameraTranslationY()
    {
        if (mJson.find(Constants::SCENE_CAMERA_TRANSLATION) == mJson.end())
        {
            setCameraTranslation(Vector3(0.0f));
        }
        return mJson[Constants::SCENE_CAMERA_TRANSLATION][Constants::Y];
    }

    float SceneDefinition::getCameraTranslationZ()
    {
        if (mJson.find(Constants::SCENE_CAMERA_TRANSLATION) == mJson.end())
        {
            setCameraTranslation(Vector3(0.0f));
        }
        return mJson[Constants::SCENE_CAMERA_TRANSLATION][Constants::Z];

    }

    void SceneDefinition::setCameraTranslationX(float val)
    {
        if (mJson.find(Constants::SCENE_CAMERA_TRANSLATION) == mJson.end())
        {
            setCameraTranslation(Vector3(0.0f));
        }
        mJson[Constants::SCENE_CAMERA_TRANSLATION][Constants::X] = val;

    }

    void SceneDefinition::setCameraTranslationY(float val)
    {
        if (mJson.find(Constants::SCENE_CAMERA_TRANSLATION) == mJson.end())
        {
            setCameraTranslation(Vector3(0.0f));
        }
        mJson[Constants::SCENE_CAMERA_TRANSLATION][Constants::Y] = val;

    }

    void SceneDefinition::setCameraTranslationZ(float val)
    {
        if (mJson.find(Constants::SCENE_CAMERA_TRANSLATION) == mJson.end())
        {
            setCameraTranslation(Vector3(0.0f));
        }
        mJson[Constants::SCENE_CAMERA_TRANSLATION][Constants::Z] = val;
    }

    void SceneDefinition::setCameraPitch(float pitch)
    {
        mJson[Constants::SCENE_CAMERA_PITCH] = pitch;
    }

    void SceneDefinition::setCameraYaw(float yaw)
    {
        mJson[Constants::SCENE_CAMERA_YAW] = yaw;
    }

    float SceneDefinition::getCameraPitch()
    {
        if (mJson.find(Constants::SCENE_CAMERA_PITCH) == mJson.end())
        {
            mJson[Constants::SCENE_CAMERA_PITCH] = 0.0f;
        }
        return  mJson[Constants::SCENE_CAMERA_PITCH];

    }

    float SceneDefinition::getCameraYaw()
    {
        if (mJson.find(Constants::SCENE_CAMERA_YAW) == mJson.end())
        {
            mJson[Constants::SCENE_CAMERA_YAW] = 0.0f;
        }
        return  mJson[Constants::SCENE_CAMERA_YAW];

    }

    UuidType
    SceneDefinition::getCameraFocusedOn
    ()
    {
        if (mJson.find(Constants::SCENE_CAMERA_FOCUSED_ON) == mJson.end())
        {
            mJson[Constants::SCENE_CAMERA_FOCUSED_ON] = 0;
        }
        return  mJson[Constants::SCENE_CAMERA_FOCUSED_ON];


    }

    void
    SceneDefinition::setCameraFocusedOn
    (UuidType focusedOn)
    {
        mJson[Constants::SCENE_CAMERA_FOCUSED_ON] = focusedOn;
    }

    UuidType
    SceneDefinition::getLightingPassShader
    ()
    {
        if (mJson.find(Constants::SCENE_LIGHTING_PASS_SHADER) == mJson.end())
        {
            mJson[Constants::SCENE_LIGHTING_PASS_SHADER] = 0;
        }
        return  mJson[Constants::SCENE_LIGHTING_PASS_SHADER];


    }

    void
    SceneDefinition::setLightingPassShader
    (UuidType shader)
    {
        mJson[Constants::SCENE_LIGHTING_PASS_SHADER] = shader;
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
}
