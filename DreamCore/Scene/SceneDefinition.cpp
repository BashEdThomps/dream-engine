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

using std::unique_lock;

namespace octronic::dream
{
    SceneDefinition::SceneDefinition
    (ProjectDefinition* projectDefinition, const json& data)
        : Definition("SceneDefinition",data),
          mRootEntityDefinition(nullptr),
          mProjectDefinition(projectDefinition)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        LOG_TRACE( "SceneDefinition: Constructing {}", getNameAndUuidString() );
    }

    SceneDefinition::~SceneDefinition
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        json rsoJson = mJson[Constants::SCENE_ROOT_ENTITY];
        if (rsoJson.is_null())
        {
            LOG_ERROR( "SceneDefinition: No root Entity found!!" );
            return;
        }

        mRootEntityDefinition = new EntityDefinition
                (
                    nullptr,
                    this,
                    rsoJson
                    );
        mRootEntityDefinition->loadChildEntityDefinitions();
    }

    void
    SceneDefinition::setCameraMovementSpeed
    (float speed)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        mJson[Constants::SCENE_CAMERA_MOVEMENT_SPEED] = speed;
    }

    float
    SceneDefinition::getCameraMovementSpeed
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        mJson[Constants::SCENE_PHYSICS_DEBUG] = debug;
    }

    bool
    SceneDefinition::getPhysicsDebug
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        if (mJson[Constants::SCENE_PHYSICS_DEBUG].is_null())
        {
            mJson[Constants::SCENE_PHYSICS_DEBUG] = false;
        }
        return mJson[Constants::SCENE_PHYSICS_DEBUG];
    }

    void SceneDefinition::setMeshCullDistance(float mcd)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        mJson[Constants::SCENE_MESH_CULL_DISTANCE] = mcd;
    }

    float SceneDefinition::getMeshCullDistance()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        if (mJson[Constants::SCENE_MESH_CULL_DISTANCE].is_null())
        {
            mJson[Constants::SCENE_MESH_CULL_DISTANCE] = 1000.0f;
        }
        return mJson[Constants::SCENE_MESH_CULL_DISTANCE];
    }

    void
    SceneDefinition::addTemplate
    (EntityDefinition* _template)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        mTemplates.push_back(_template);
    }


    EntityDefinition*
    SceneDefinition::getTemplateByUuid
    (UuidType uuid)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        // Translation
        mJson[Constants::SCENE_CAMERA_TRANSLATION] = wrapVector3(transform);
    }

    Vector3 SceneDefinition::getCameraLookAt()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        mJson[Constants::SCENE_CAMERA_LOOK_AT][Constants::X] = wrapVector3(lookAt);
    }

    Vector3
    SceneDefinition::getGravity
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        mJson[Constants::SCENE_GRAVITY] = wrapVector3(gravity);
    }

    void
    SceneDefinition::setGravityX
    (float x)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        mJson[Constants::SCENE_GRAVITY][Constants::X] = x;
    }

    void
    SceneDefinition::setGravityY
    (float y)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        mJson[Constants::SCENE_GRAVITY][Constants::Y] = y;
    }

    void
    SceneDefinition::setGravityZ
    (float z)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        mJson[Constants::SCENE_GRAVITY][Constants::Z] = z;
    }

    Vector3
    SceneDefinition::getClearColour
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        mJson[Constants::SCENE_CLEAR_COLOUR][Constants::RED]  = r;
    }

    void
    SceneDefinition::setClearColourG
    (float g)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        mJson[Constants::SCENE_CLEAR_COLOUR][Constants::GREEN]  = g;
    }

    void
    SceneDefinition::setClearColourB
    (float b)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        mJson[Constants::SCENE_CLEAR_COLOUR][Constants::BLUE]  = b;
    }

    EntityDefinition*
    SceneDefinition::getRootEntityDefinition
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        return mRootEntityDefinition;
    }

    ProjectDefinition*
    SceneDefinition::getProjectDefinition
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        return mProjectDefinition;
    }

    EntityDefinition*
    SceneDefinition::createNewRootEntityDefinition
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        json rootDefJson;
        rootDefJson[Constants::NAME] = Constants::ENTITY_ROOT_NAME;
        rootDefJson[Constants::UUID] = Uuid::generateUuid();
        Transform transform;
        rootDefJson[Constants::TRANSFORM] = transform.getJson();
        mRootEntityDefinition = new EntityDefinition
                (
                    nullptr,
                    this,
                    rootDefJson
                    );
        return mRootEntityDefinition;
    }

    json
    SceneDefinition::getJson
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        mJson[Constants::SCENE_ROOT_ENTITY] = mRootEntityDefinition->getJson();
        return mJson;
    }

    void SceneDefinition::setMinDrawDistance(float mdd)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        mJson[Constants::SCENE_MIN_DRAW_DISTANCE] = mdd;
    }

    float SceneDefinition::getMinDrawDistance()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        if (mJson[Constants::SCENE_MIN_DRAW_DISTANCE].is_null())
        {
            mJson[Constants::SCENE_MIN_DRAW_DISTANCE] = 0.1f;
        }
        return mJson[Constants::SCENE_MIN_DRAW_DISTANCE];
    }

    void SceneDefinition::setMaxDrawDistance(float mdd)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        mJson[Constants::SCENE_MAX_DRAW_DISTANCE] = mdd;
    }

    float SceneDefinition::getMaxDrawDistance()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        if (mJson[Constants::SCENE_MAX_DRAW_DISTANCE].is_null())
        {
            mJson[Constants::SCENE_MAX_DRAW_DISTANCE] = 1000.0f;
        }
        return mJson[Constants::SCENE_MAX_DRAW_DISTANCE];
    }

    float SceneDefinition::getCameraTranslationX()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        if (mJson[Constants::SCENE_CAMERA_TRANSLATION].is_null())
        {
            setCameraTranslation(Vector3(0.0f));
        }
        return mJson[Constants::SCENE_CAMERA_TRANSLATION][Constants::X];
    }

    float SceneDefinition::getCameraTranslationY()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        if (mJson[Constants::SCENE_CAMERA_TRANSLATION].is_null())
        {
            setCameraTranslation(Vector3(0.0f));
        }
        return mJson[Constants::SCENE_CAMERA_TRANSLATION][Constants::Y];
    }

    float SceneDefinition::getCameraTranslationZ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        if (mJson[Constants::SCENE_CAMERA_TRANSLATION].is_null())
        {
            setCameraTranslation(Vector3(0.0f));
        }
        return mJson[Constants::SCENE_CAMERA_TRANSLATION][Constants::Z];
    }

    void SceneDefinition::setCameraTranslationX(float val)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        if (mJson[Constants::SCENE_CAMERA_TRANSLATION].is_null())
        {
            setCameraTranslation(Vector3(0.0f));
        }
        mJson[Constants::SCENE_CAMERA_TRANSLATION][Constants::X] = val;
    }

    void SceneDefinition::setCameraTranslationY(float val)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        if (mJson[Constants::SCENE_CAMERA_TRANSLATION].is_null())
        {
            setCameraTranslation(Vector3(0.0f));
        }
        mJson[Constants::SCENE_CAMERA_TRANSLATION][Constants::Y] = val;
    }

    void SceneDefinition::setCameraTranslationZ(float val)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        if (mJson[Constants::SCENE_CAMERA_TRANSLATION].is_null())
        {
            setCameraTranslation(Vector3(0.0f));
        }
        mJson[Constants::SCENE_CAMERA_TRANSLATION][Constants::Z] = val;
    }

    void SceneDefinition::setCameraPitch(float pitch)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        mJson[Constants::SCENE_CAMERA_PITCH] = pitch;
    }

    void SceneDefinition::setCameraYaw(float yaw)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        mJson[Constants::SCENE_CAMERA_YAW] = yaw;
    }

    float SceneDefinition::getCameraPitch()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        if (mJson[Constants::SCENE_CAMERA_PITCH].is_null())
        {
            mJson[Constants::SCENE_CAMERA_PITCH] = 0.0f;
        }
        return  mJson[Constants::SCENE_CAMERA_PITCH];
    }

    float SceneDefinition::getCameraYaw()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        if (mJson[Constants::SCENE_CAMERA_YAW].is_null())
        {
            mJson[Constants::SCENE_CAMERA_YAW] = 0.0f;
        }
        return  mJson[Constants::SCENE_CAMERA_YAW];
    }

    UuidType
    SceneDefinition::getCameraFocusedOn
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        if (!mJson[Constants::SCENE_CAMERA_FOCUSED_ON].is_number())
        {
            mJson[Constants::SCENE_CAMERA_FOCUSED_ON] = 0;
        }
        return  mJson[Constants::SCENE_CAMERA_FOCUSED_ON];

    }

    void
    SceneDefinition::setCameraFocusedOn
    (UuidType focusedOn)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        mJson[Constants::SCENE_CAMERA_FOCUSED_ON] = focusedOn;
    }

    UuidType
    SceneDefinition::getLightingPassShader
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        if (!mJson[Constants::SCENE_LIGHTING_PASS_SHADER].is_number())
        {
            mJson[Constants::SCENE_LIGHTING_PASS_SHADER] = 0;
        }
        return  mJson[Constants::SCENE_LIGHTING_PASS_SHADER];

    }

    void
    SceneDefinition::setLightingPassShader
    (UuidType shader)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        mJson[Constants::SCENE_LIGHTING_PASS_SHADER] = shader;
    }

    UuidType
    SceneDefinition::getShadowPassShader
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        if (!mJson[Constants::SCENE_SHADOW_PASS_SHADER].is_number())
        {
            mJson[Constants::SCENE_SHADOW_PASS_SHADER] = 0;
        }
        return  mJson[Constants::SCENE_SHADOW_PASS_SHADER];

    }

    void
    SceneDefinition::setShadowPassShader
    (UuidType shader)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        mJson[Constants::SCENE_SHADOW_PASS_SHADER] = shader;
    }

    UuidType
    SceneDefinition::getFontShader
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        if (!mJson[Constants::SCENE_FONT_SHADER].is_number())
        {
            mJson[Constants::SCENE_FONT_SHADER] = 0;
        }
        return  mJson[Constants::SCENE_FONT_SHADER];

    }

    void
    SceneDefinition::setFontShader
    (UuidType shader)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        mJson[Constants::SCENE_FONT_SHADER] = shader;
    }

    UuidType
    SceneDefinition::getInputScript
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        if (!mJson[Constants::SCENE_INPUT_SCRIPT].is_number())
        {
            mJson[Constants::SCENE_INPUT_SCRIPT] = 0;
        }
        return  mJson[Constants::SCENE_INPUT_SCRIPT];

    }

    void
    SceneDefinition::setInputScript
    (UuidType shader)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        mJson[Constants::SCENE_INPUT_SCRIPT] = shader;
    }

    void
    SceneDefinition::setPlayerObject
    (UuidType po)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        mJson[Constants::ENTITY_PLAYER_OBJECT] = po;
    }

    UuidType SceneDefinition::getPlayerObject()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        if (!mJson[Constants::ENTITY_PLAYER_OBJECT].is_number())
        {
            mJson[Constants::ENTITY_PLAYER_OBJECT] = 0;
        }
        return mJson[Constants::ENTITY_PLAYER_OBJECT];
    }
}
