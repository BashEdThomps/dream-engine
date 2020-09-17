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
#include "Actor/ActorDefinition.h"
#include "../Components/Transform.h"
#include "../Project/ProjectDefinition.h"
#include "../Common/Uuid.h"

namespace Dream
{
    SceneDefinition::SceneDefinition
    (ProjectDefinition* projectDefinition, const json& data)
        : Definition(data),
          mRootActorDefinition(nullptr),
          mProjectDefinition(projectDefinition)
    {
        #ifdef DREAM_LOG
        setLogClassName("SceneDefinition");
        auto log = getLog();
        log->trace( "Constructing {}", getNameAndUuidString() );
        #endif
    }

    SceneDefinition::~SceneDefinition
    ()
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        log->trace( "Destructing {}", getNameAndUuidString() );
        #endif
        if (mRootActorDefinition != nullptr)
        {
            delete mRootActorDefinition;
            mRootActorDefinition = nullptr;
        }
    }

    void
    SceneDefinition::loadRootActorDefinition
    ()
    {
        json rsoJson = mJson[Constants::SCENE_ROOT_ACTOR];
        if (rsoJson.is_null())
        {
            #ifdef DREAM_LOG
            auto log = getLog();
            log->error( "No root Actor found!!" );
            #endif
            return;
        }

        mRootActorDefinition = new ActorDefinition
        (
            nullptr,
            this,
            rsoJson
        );
        mRootActorDefinition->loadChildActorDefinitions();
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
        if (mJson[Constants::SCENE_CAMERA_MOVEMENT_SPEED].is_null())
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
        if (mJson[Constants::SCENE_PHYSICS_DEBUG].is_null())
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
        if (mJson[Constants::SCENE_MESH_CULL_DISTANCE].is_null())
        {
            mJson[Constants::SCENE_MESH_CULL_DISTANCE] = 1000.0f;
        }
        return mJson[Constants::SCENE_MESH_CULL_DISTANCE];
    }

    void
    SceneDefinition::addTemplate
    (ActorDefinition* _template)
    {
        mTemplates.push_back(_template);
    }


    ActorDefinition*
    SceneDefinition::getTemplateByUuid
    (uint32_t uuid)
    {
        for (ActorDefinition* next : mTemplates)
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
        if (mJson[Constants::SCENE_CAMERA_TRANSLATION].is_null())
        {
           setCameraTranslation(Vector3(0.0f));
        }
        return unwrapVector3(mJson[Constants::SCENE_CAMERA_TRANSLATION]);
    }

    void
    SceneDefinition::setCameraTranslation
    (const Vector3& transform)
    {
        // Translation
        mJson[Constants::SCENE_CAMERA_TRANSLATION] = wrapVector3(transform);
    }

    Vector3 SceneDefinition::getCameraLookAt()
    {
        if (mJson[Constants::SCENE_CAMERA_LOOK_AT].is_null())
        {
           setCameraLookAt(Vector3(0.0f));
        }
        return unwrapVector3(mJson[Constants::SCENE_CAMERA_LOOK_AT]);
    }

    void
    SceneDefinition::setCameraLookAt
    (const Vector3& lookAt)
    {
        mJson[Constants::SCENE_CAMERA_LOOK_AT][Constants::X] = wrapVector3(lookAt);
    }

    Vector3
    SceneDefinition::getGravity
    ()
    {
        Vector3 gravity;

        if (mJson[Constants::SCENE_GRAVITY].is_null())
        {
            mJson[Constants::SCENE_GRAVITY] = wrapVector3(Vector3(0.0f));
        }

        gravity = unwrapVector3(mJson[Constants::SCENE_GRAVITY]);

        return gravity;
    }

    void
    SceneDefinition::setGravity
    (const Vector3& gravity)
    {
        mJson[Constants::SCENE_GRAVITY] = wrapVector3(gravity);
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

        if (mJson[Constants::SCENE_CLEAR_COLOUR].is_null())
        {
            mJson[Constants::SCENE_CLEAR_COLOUR][Constants::RED]   = 0.0f;
            mJson[Constants::SCENE_CLEAR_COLOUR][Constants::GREEN] = 0.0f;
            mJson[Constants::SCENE_CLEAR_COLOUR][Constants::BLUE]  = 0.0f;

        }

        colour.setX(mJson[Constants::SCENE_CLEAR_COLOUR][Constants::RED]);
        colour.setY(mJson[Constants::SCENE_CLEAR_COLOUR][Constants::GREEN]);
        colour.setZ(mJson[Constants::SCENE_CLEAR_COLOUR][Constants::BLUE]);

        return colour;
    }

    void
    SceneDefinition::setClearColour
    (const Vector3& colour)
    {
        if (mJson[Constants::SCENE_CLEAR_COLOUR].is_null())
        {
            mJson[Constants::SCENE_CLEAR_COLOUR] = {};
        }

        mJson[Constants::SCENE_CLEAR_COLOUR][Constants::RED]   = colour.x();
        mJson[Constants::SCENE_CLEAR_COLOUR][Constants::GREEN] = colour.y();
        mJson[Constants::SCENE_CLEAR_COLOUR][Constants::BLUE]  = colour.z();
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

    ActorDefinition*
    SceneDefinition::getRootActorDefinition
    ()
    {
        return mRootActorDefinition;
    }

    ProjectDefinition*
    SceneDefinition::getProjectDefinition
    ()
    {
        return mProjectDefinition;
    }

    ActorDefinition*
    SceneDefinition::createNewRootActorDefinition
    ()
    {
        json rootDefJson;
        rootDefJson[Constants::NAME] = Constants::ACTOR_ROOT_NAME;
        rootDefJson[Constants::UUID] = Uuid::generateUuid();
        Transform transform;
        rootDefJson[Constants::TRANSFORM] = transform.getJson();
        mRootActorDefinition = new ActorDefinition
        (
            nullptr,
            this,
            rootDefJson
        );
        return mRootActorDefinition;
    }

    json
    SceneDefinition::getJson
    ()
    {
        mJson[Constants::SCENE_ROOT_ACTOR] = mRootActorDefinition->getJson();
        return mJson;
    }

    void SceneDefinition::setMinDrawDistance(float mdd)
    {
       mJson[Constants::SCENE_MIN_DRAW_DISTANCE] = mdd;
    }

    float SceneDefinition::getMinDrawDistance()
    {
        if (mJson[Constants::SCENE_MIN_DRAW_DISTANCE].is_null())
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
        if (mJson[Constants::SCENE_MAX_DRAW_DISTANCE].is_null())
        {
           mJson[Constants::SCENE_MAX_DRAW_DISTANCE] = 1000.0f;
        }
        return mJson[Constants::SCENE_MAX_DRAW_DISTANCE];
    }

    float SceneDefinition::getCameraTranslationX()
    {
        if (mJson[Constants::SCENE_CAMERA_TRANSLATION].is_null())
        {
           setCameraTranslation(Vector3(0.0f));
        }
        return mJson[Constants::SCENE_CAMERA_TRANSLATION][Constants::X];
    }

    float SceneDefinition::getCameraTranslationY()
    {
        if (mJson[Constants::SCENE_CAMERA_TRANSLATION].is_null())
        {
           setCameraTranslation(Vector3(0.0f));
        }
        return mJson[Constants::SCENE_CAMERA_TRANSLATION][Constants::Y];
    }

    float SceneDefinition::getCameraTranslationZ()
    {
        if (mJson[Constants::SCENE_CAMERA_TRANSLATION].is_null())
        {
           setCameraTranslation(Vector3(0.0f));
        }
        return mJson[Constants::SCENE_CAMERA_TRANSLATION][Constants::Z];
    }

    void SceneDefinition::setCameraTranslationX(float val)
    {
        if (mJson[Constants::SCENE_CAMERA_TRANSLATION].is_null())
        {
           setCameraTranslation(Vector3(0.0f));
        }
        mJson[Constants::SCENE_CAMERA_TRANSLATION][Constants::X] = val;
    }

    void SceneDefinition::setCameraTranslationY(float val)
    {
        if (mJson[Constants::SCENE_CAMERA_TRANSLATION].is_null())
        {
           setCameraTranslation(Vector3(0.0f));
        }
        mJson[Constants::SCENE_CAMERA_TRANSLATION][Constants::Y] = val;
    }

    void SceneDefinition::setCameraTranslationZ(float val)
    {
        if (mJson[Constants::SCENE_CAMERA_TRANSLATION].is_null())
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
       if (mJson[Constants::SCENE_CAMERA_PITCH].is_null())
       {
           mJson[Constants::SCENE_CAMERA_PITCH] = 0.0f;
       }
       return  mJson[Constants::SCENE_CAMERA_PITCH];
    }

    float SceneDefinition::getCameraYaw()
    {
       if (mJson[Constants::SCENE_CAMERA_YAW].is_null())
       {
           mJson[Constants::SCENE_CAMERA_YAW] = 0.0f;
       }
       return  mJson[Constants::SCENE_CAMERA_YAW];
    }

    uint32_t
    SceneDefinition::getCameraFocusedOn
    ()
    {
       if (!mJson[Constants::SCENE_CAMERA_FOCUSED_ON].is_number())
       {
           mJson[Constants::SCENE_CAMERA_FOCUSED_ON] = 0;
       }
       return  mJson[Constants::SCENE_CAMERA_FOCUSED_ON];

    }

    void
    SceneDefinition::setCameraFocusedOn
    (uint32_t focusedOn)
    {
        mJson[Constants::SCENE_CAMERA_FOCUSED_ON] = focusedOn;
    }

    uint32_t
    SceneDefinition::getLightingPassShader
    ()
    {
       if (!mJson[Constants::SCENE_LIGHTING_PASS_SHADER].is_number())
       {
           mJson[Constants::SCENE_LIGHTING_PASS_SHADER] = 0;
       }
       return  mJson[Constants::SCENE_LIGHTING_PASS_SHADER];

    }

    void
    SceneDefinition::setLightingPassShader
    (uint32_t shader)
    {
       mJson[Constants::SCENE_LIGHTING_PASS_SHADER] = shader;
    }

    uint32_t
    SceneDefinition::getShadowPassShader
    ()
    {
       if (!mJson[Constants::SCENE_SHADOW_PASS_SHADER].is_number())
       {
           mJson[Constants::SCENE_SHADOW_PASS_SHADER] = 0;
       }
       return  mJson[Constants::SCENE_SHADOW_PASS_SHADER];

    }

    void
    SceneDefinition::setShadowPassShader
    (uint32_t shader)
    {
       mJson[Constants::SCENE_SHADOW_PASS_SHADER] = shader;
    }

    uint32_t
    SceneDefinition::getInputScript
    ()
    {
       if (!mJson[Constants::SCENE_INPUT_SCRIPT].is_number())
       {
           mJson[Constants::SCENE_INPUT_SCRIPT] = 0;
       }
       return  mJson[Constants::SCENE_INPUT_SCRIPT];

    }

    void
    SceneDefinition::setInputScript
    (uint32_t shader)
    {
       mJson[Constants::SCENE_INPUT_SCRIPT] = shader;
    }

    void
    SceneDefinition::setPlayerObject
    (uint32_t po)
    {
        mJson[Constants::ACTOR_PLAYER_OBJECT] = po;
    }

    uint32_t SceneDefinition::getPlayerObject()
    {
        if (!mJson[Constants::ACTOR_PLAYER_OBJECT].is_number())
        {
            mJson[Constants::ACTOR_PLAYER_OBJECT] = 0;
        }
        return mJson[Constants::ACTOR_PLAYER_OBJECT];
    }
}
