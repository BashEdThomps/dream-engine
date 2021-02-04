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
        if(dreamTryLock()) {
            dreamLock();

            if (mJson.find(Constants::SCENE_ROOT_ENTITY) == mJson.end())
            {
                LOG_ERROR( "SceneDefinition: No root Entity found!!" );
                return;
            }
            json rsoJson = mJson[Constants::SCENE_ROOT_ENTITY];

            mRootEntityDefinition = new EntityDefinition(nullptr, this, rsoJson );
            mRootEntityDefinition->loadChildEntityDefinitions();
        } dreamElseLockFailed
    }

    void
    SceneDefinition::setCameraMovementSpeed
    (float speed)
    {
        if(dreamTryLock()) {
            dreamLock();

            mJson[Constants::SCENE_CAMERA_MOVEMENT_SPEED] = speed;
        } dreamElseLockFailed
    }

    float
    SceneDefinition::getCameraMovementSpeed
    ()
    {
        if(dreamTryLock()) {
            dreamLock();

            if (mJson.find(Constants::SCENE_CAMERA_MOVEMENT_SPEED) == mJson.end())
            {
                mJson[Constants::SCENE_CAMERA_MOVEMENT_SPEED] = Constants::SCENE_CAMERA_DEFAULT_MOVEMENT_SPEED;
            }
            return mJson[Constants::SCENE_CAMERA_MOVEMENT_SPEED];
        } dreamElseLockFailed
    }

    void
    SceneDefinition::setPhysicsDebug
    (bool debug)
    {
        if(dreamTryLock()) {
            dreamLock();

            mJson[Constants::SCENE_PHYSICS_DEBUG] = debug;
        } dreamElseLockFailed
    }

    bool
    SceneDefinition::getPhysicsDebug
    ()
    {
        if(dreamTryLock()) {
            dreamLock();

            if (mJson.find(Constants::SCENE_PHYSICS_DEBUG) == mJson.end())
            {
                mJson[Constants::SCENE_PHYSICS_DEBUG] = false;
            }
            return mJson[Constants::SCENE_PHYSICS_DEBUG];
        } dreamElseLockFailed
    }

    void SceneDefinition::setMeshCullDistance(float mcd)
    {
        if(dreamTryLock()) {
            dreamLock();

            mJson[Constants::SCENE_MESH_CULL_DISTANCE] = mcd;
        } dreamElseLockFailed
    }

    float SceneDefinition::getMeshCullDistance()
    {
        if(dreamTryLock()) {
            dreamLock();

            if (mJson.find(Constants::SCENE_MESH_CULL_DISTANCE) == mJson.end())
            {
                mJson[Constants::SCENE_MESH_CULL_DISTANCE] = 1000.0f;
            }
            return mJson[Constants::SCENE_MESH_CULL_DISTANCE];
        } dreamElseLockFailed
    }

    void
    SceneDefinition::addTemplate
    (EntityDefinition* _template)
    {
        if(dreamTryLock()) {
            dreamLock();

            mTemplates.push_back(_template);
        } dreamElseLockFailed
    }


    EntityDefinition*
    SceneDefinition::getTemplateByUuid
    (UuidType uuid)
    {
        if(dreamTryLock()) {
            dreamLock();

            for (EntityDefinition* next : mTemplates)
            {
                if (next->getUuid() == uuid)
                {
                    return next;
                }
            }
            return nullptr;
        } dreamElseLockFailed
    }

    Vector3
    SceneDefinition::getCameraTranslation
    ()
    {
        if(dreamTryLock()) {
            dreamLock();

            if (mJson.find(Constants::SCENE_CAMERA_TRANSLATION) == mJson.end())
            {
                setCameraTranslation(Vector3(0.0f));
            }
            return Vector3(mJson[Constants::SCENE_CAMERA_TRANSLATION]);
        } dreamElseLockFailed
    }

    void
    SceneDefinition::setCameraTranslation
    (const Vector3& transform)
    {
        if(dreamTryLock()) {
            dreamLock();

            // Translation
            mJson[Constants::SCENE_CAMERA_TRANSLATION] = transform.toJson();
        } dreamElseLockFailed
    }

    Vector3 SceneDefinition::getCameraLookAt()
    {
        if(dreamTryLock()) {
            dreamLock();

            if (mJson.find(Constants::SCENE_CAMERA_LOOK_AT) == mJson.end())
            {
                setCameraLookAt(Vector3(0.0f));
            }
            return Vector3(mJson[Constants::SCENE_CAMERA_LOOK_AT]);
        } dreamElseLockFailed
    }

    void
    SceneDefinition::setCameraLookAt
    (const Vector3& lookAt)
    {
        if(dreamTryLock()) {
            dreamLock();

            mJson[Constants::SCENE_CAMERA_LOOK_AT] = lookAt.toJson();
        } dreamElseLockFailed
    }

    Vector3
    SceneDefinition::getGravity
    ()
    {
        if(dreamTryLock()) {
            dreamLock();

            Vector3 gravity;

            if (mJson.find(Constants::SCENE_GRAVITY) == mJson.end())
            {
                mJson[Constants::SCENE_GRAVITY] = Vector3(0.0f).toJson();
            }

            gravity = Vector3(mJson[Constants::SCENE_GRAVITY]);

            return gravity;
        } dreamElseLockFailed
    }

    void
    SceneDefinition::setGravity
    (const Vector3& gravity)
    {
        if(dreamTryLock()) {
            dreamLock();

            mJson[Constants::SCENE_GRAVITY] = gravity.toJson();
        } dreamElseLockFailed
    }

    void
    SceneDefinition::setGravityX
    (float x)
    {
        if(dreamTryLock()) {
            dreamLock();

            mJson[Constants::SCENE_GRAVITY][Constants::X] = x;
        } dreamElseLockFailed
    }

    void
    SceneDefinition::setGravityY
    (float y)
    {
        if(dreamTryLock()) {
            dreamLock();

            mJson[Constants::SCENE_GRAVITY][Constants::Y] = y;
        } dreamElseLockFailed
    }

    void
    SceneDefinition::setGravityZ
    (float z)
    {
        if(dreamTryLock()) {
            dreamLock();

            mJson[Constants::SCENE_GRAVITY][Constants::Z] = z;
        } dreamElseLockFailed
    }

    Vector3
    SceneDefinition::getClearColour
    ()
    {
        if(dreamTryLock()) {
            dreamLock();

            Vector3 colour;

            if (mJson.find(Constants::SCENE_CLEAR_COLOUR) == mJson.end())
            {
                mJson[Constants::SCENE_CLEAR_COLOUR] = Vector3(0.f).toJson();
            }

            colour = Vector3(mJson[Constants::SCENE_CLEAR_COLOUR]);
            return colour;
        } dreamElseLockFailed
    }

    void
    SceneDefinition::setClearColour
    (const Vector3& colour)
    {
        if(dreamTryLock()) {
            dreamLock();

            if (mJson.find(Constants::SCENE_CLEAR_COLOUR) == mJson.end())
            {
                mJson[Constants::SCENE_CLEAR_COLOUR] = Vector3(0.f).toJson();
            }

            mJson[Constants::SCENE_CLEAR_COLOUR] = colour.toJson();
        } dreamElseLockFailed
    }

    void
    SceneDefinition::setClearColourR
    (float r)
    {
        if(dreamTryLock()) {
            dreamLock();

            mJson[Constants::SCENE_CLEAR_COLOUR][Constants::RED]  = r;
        } dreamElseLockFailed
    }

    void
    SceneDefinition::setClearColourG
    (float g)
    {
        if(dreamTryLock()) {
            dreamLock();

            mJson[Constants::SCENE_CLEAR_COLOUR][Constants::GREEN]  = g;
        } dreamElseLockFailed
    }

    void
    SceneDefinition::setClearColourB
    (float b)
    {
        if(dreamTryLock()) {
            dreamLock();

            mJson[Constants::SCENE_CLEAR_COLOUR][Constants::BLUE]  = b;
        } dreamElseLockFailed
    }

    EntityDefinition*
    SceneDefinition::getRootEntityDefinition
    ()
    {
        if(dreamTryLock()) {
            dreamLock();

            return mRootEntityDefinition;
        } dreamElseLockFailed
    }

    ProjectDefinition*
    SceneDefinition::getProjectDefinition
    ()
    {
        if(dreamTryLock()) {
            dreamLock();

            return mProjectDefinition;
        } dreamElseLockFailed
    }

    EntityDefinition*
    SceneDefinition::createNewRootEntityDefinition
    ()
    {
        if(dreamTryLock()) {
            dreamLock();

            json rootDefJson;
            rootDefJson[Constants::NAME] = Constants::ENTITY_ROOT_NAME;
            rootDefJson[Constants::UUID] = Uuid::generateUuid();
            Transform transform;
            rootDefJson[Constants::TRANSFORM] = transform.getJson();
            mRootEntityDefinition = new EntityDefinition(nullptr,this,rootDefJson);
            return mRootEntityDefinition;
        } dreamElseLockFailed
    }

    json
    SceneDefinition::getJson
    ()
    {
        if(dreamTryLock()) {
            dreamLock();

            mJson[Constants::SCENE_ROOT_ENTITY] = mRootEntityDefinition->getJson();
            return mJson;
        } dreamElseLockFailed
    }

    void SceneDefinition::setMinDrawDistance(float mdd)
    {
        if(dreamTryLock()) {
            dreamLock();

            mJson[Constants::SCENE_MIN_DRAW_DISTANCE] = mdd;
        } dreamElseLockFailed
    }

    float SceneDefinition::getMinDrawDistance()
    {
        if(dreamTryLock()) {
            dreamLock();

            if (mJson.find(Constants::SCENE_MIN_DRAW_DISTANCE) == mJson.end())
            {
                mJson[Constants::SCENE_MIN_DRAW_DISTANCE] = 0.1f;
            }
            return mJson[Constants::SCENE_MIN_DRAW_DISTANCE];
        } dreamElseLockFailed
    }

    void SceneDefinition::setMaxDrawDistance(float mdd)
    {
        if(dreamTryLock()) {
            dreamLock();

            mJson[Constants::SCENE_MAX_DRAW_DISTANCE] = mdd;
        } dreamElseLockFailed
    }

    float SceneDefinition::getMaxDrawDistance()
    {
        if(dreamTryLock()) {
            dreamLock();

            if (mJson.find(Constants::SCENE_MAX_DRAW_DISTANCE) == mJson.end())
            {
                mJson[Constants::SCENE_MAX_DRAW_DISTANCE] = 1000.0f;
            }
            return mJson[Constants::SCENE_MAX_DRAW_DISTANCE];
        } dreamElseLockFailed
    }

    float SceneDefinition::getCameraTranslationX()
    {
        if(dreamTryLock()) {
            dreamLock();

            if (mJson.find(Constants::SCENE_CAMERA_TRANSLATION) == mJson.end())
            {
                setCameraTranslation(Vector3(0.0f));
            }
            return mJson[Constants::SCENE_CAMERA_TRANSLATION][Constants::X];
        } dreamElseLockFailed
    }

    float SceneDefinition::getCameraTranslationY()
    {
        if(dreamTryLock()) {
            dreamLock();

            if (mJson.find(Constants::SCENE_CAMERA_TRANSLATION) == mJson.end())
            {
                setCameraTranslation(Vector3(0.0f));
            }
            return mJson[Constants::SCENE_CAMERA_TRANSLATION][Constants::Y];
        } dreamElseLockFailed
    }

    float SceneDefinition::getCameraTranslationZ()
    {
        if(dreamTryLock()) {
            dreamLock();

            if (mJson.find(Constants::SCENE_CAMERA_TRANSLATION) == mJson.end())
            {
                setCameraTranslation(Vector3(0.0f));
            }
            return mJson[Constants::SCENE_CAMERA_TRANSLATION][Constants::Z];
        } dreamElseLockFailed
    }

    void SceneDefinition::setCameraTranslationX(float val)
    {
        if(dreamTryLock()) {
            dreamLock();

            if (mJson.find(Constants::SCENE_CAMERA_TRANSLATION) == mJson.end())
            {
                setCameraTranslation(Vector3(0.0f));
            }
            mJson[Constants::SCENE_CAMERA_TRANSLATION][Constants::X] = val;
        } dreamElseLockFailed
    }

    void SceneDefinition::setCameraTranslationY(float val)
    {
        if(dreamTryLock()) {
            dreamLock();

            if (mJson.find(Constants::SCENE_CAMERA_TRANSLATION) == mJson.end())
            {
                setCameraTranslation(Vector3(0.0f));
            }
            mJson[Constants::SCENE_CAMERA_TRANSLATION][Constants::Y] = val;
        } dreamElseLockFailed
    }

    void SceneDefinition::setCameraTranslationZ(float val)
    {
        if(dreamTryLock()) {
            dreamLock();

            if (mJson.find(Constants::SCENE_CAMERA_TRANSLATION) == mJson.end())
            {
                setCameraTranslation(Vector3(0.0f));
            }
            mJson[Constants::SCENE_CAMERA_TRANSLATION][Constants::Z] = val;
        } dreamElseLockFailed
    }

    void SceneDefinition::setCameraPitch(float pitch)
    {
        if(dreamTryLock()) {
            dreamLock();

            mJson[Constants::SCENE_CAMERA_PITCH] = pitch;
        } dreamElseLockFailed
    }

    void SceneDefinition::setCameraYaw(float yaw)
    {
        if(dreamTryLock()) {
            dreamLock();

            mJson[Constants::SCENE_CAMERA_YAW] = yaw;
        } dreamElseLockFailed
    }

    float SceneDefinition::getCameraPitch()
    {
        if(dreamTryLock()) {
            dreamLock();

            if (mJson.find(Constants::SCENE_CAMERA_PITCH) == mJson.end())
            {
                mJson[Constants::SCENE_CAMERA_PITCH] = 0.0f;
            }
            return  mJson[Constants::SCENE_CAMERA_PITCH];
        } dreamElseLockFailed
    }

    float SceneDefinition::getCameraYaw()
    {
        if(dreamTryLock()) {
            dreamLock();

            if (mJson.find(Constants::SCENE_CAMERA_YAW) == mJson.end())
            {
                mJson[Constants::SCENE_CAMERA_YAW] = 0.0f;
            }
            return  mJson[Constants::SCENE_CAMERA_YAW];
        } dreamElseLockFailed
    }

    UuidType
    SceneDefinition::getCameraFocusedOn
    ()
    {
        if(dreamTryLock()) {
            dreamLock();

            if (!mJson[Constants::SCENE_CAMERA_FOCUSED_ON].is_number())
            {
                mJson[Constants::SCENE_CAMERA_FOCUSED_ON] = 0;
            }
            return  mJson[Constants::SCENE_CAMERA_FOCUSED_ON];
        } dreamElseLockFailed

    }

    void
    SceneDefinition::setCameraFocusedOn
    (UuidType focusedOn)
    {
        if(dreamTryLock()) {
            dreamLock();

            mJson[Constants::SCENE_CAMERA_FOCUSED_ON] = focusedOn;
        } dreamElseLockFailed
    }

    UuidType
    SceneDefinition::getLightingPassShader
    ()
    {
        if(dreamTryLock()) {
            dreamLock();

            if (!mJson[Constants::SCENE_LIGHTING_PASS_SHADER].is_number())
            {
                mJson[Constants::SCENE_LIGHTING_PASS_SHADER] = 0;
            }
            return  mJson[Constants::SCENE_LIGHTING_PASS_SHADER];
        } dreamElseLockFailed

    }

    void
    SceneDefinition::setLightingPassShader
    (UuidType shader)
    {
        if(dreamTryLock()) {
            dreamLock();

            mJson[Constants::SCENE_LIGHTING_PASS_SHADER] = shader;
        } dreamElseLockFailed
    }

    UuidType
    SceneDefinition::getShadowPassShader
    ()
    {
        if(dreamTryLock()) {
            dreamLock();

            if (!mJson[Constants::SCENE_SHADOW_PASS_SHADER].is_number())
            {
                mJson[Constants::SCENE_SHADOW_PASS_SHADER] = 0;
            }
            return  mJson[Constants::SCENE_SHADOW_PASS_SHADER];
        } dreamElseLockFailed

    }

    void
    SceneDefinition::setShadowPassShader
    (UuidType shader)
    {
        if(dreamTryLock()) {
            dreamLock();

            mJson[Constants::SCENE_SHADOW_PASS_SHADER] = shader;
        } dreamElseLockFailed
    }

    UuidType
    SceneDefinition::getFontShader
    ()
    {
        if(dreamTryLock()) {
            dreamLock();

            if (!mJson[Constants::SCENE_FONT_SHADER].is_number())
            {
                mJson[Constants::SCENE_FONT_SHADER] = 0;
            }
            return  mJson[Constants::SCENE_FONT_SHADER];
        } dreamElseLockFailed

    }

    void
    SceneDefinition::setFontShader
    (UuidType shader)
    {
        if(dreamTryLock()) {
            dreamLock();

            mJson[Constants::SCENE_FONT_SHADER] = shader;
        } dreamElseLockFailed
    }

    UuidType
    SceneDefinition::getInputScript
    ()
    {
        if(dreamTryLock()) {
            dreamLock();

            if (!mJson[Constants::SCENE_INPUT_SCRIPT].is_number())
            {
                mJson[Constants::SCENE_INPUT_SCRIPT] = 0;
            }
            return  mJson[Constants::SCENE_INPUT_SCRIPT];
        } dreamElseLockFailed

    }

    void
    SceneDefinition::setInputScript
    (UuidType shader)
    {
        if(dreamTryLock()) {
            dreamLock();

            mJson[Constants::SCENE_INPUT_SCRIPT] = shader;
        } dreamElseLockFailed
    }

    void
    SceneDefinition::setPlayerObject
    (UuidType po)
    {
        if(dreamTryLock()) {
            dreamLock();

            mJson[Constants::ENTITY_PLAYER_OBJECT] = po;
        } dreamElseLockFailed
    }

    UuidType SceneDefinition::getPlayerObject()
    {
        if(dreamTryLock()) {
            dreamLock();

            if (!mJson[Constants::ENTITY_PLAYER_OBJECT].is_number())
            {
                mJson[Constants::ENTITY_PLAYER_OBJECT] = 0;
            }
            return mJson[Constants::ENTITY_PLAYER_OBJECT];
        } dreamElseLockFailed
    }
}
