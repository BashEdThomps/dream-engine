/*
 * SceneDefinition.cpp
 *
 * Created: 16 2017 by Ashley
 *
 * Copyright 2017 Octronic. All rights reserved.
 *
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
#include "SceneObject/SceneObjectDefinition.h"
#include "../Components/Transform.h"
#include "../Project/ProjectDefinition.h"
#include "../Utilities/String.h"
#include "../Utilities/Uuid.h"

namespace Dream
{
    SceneDefinition::SceneDefinition
    (ProjectDefinition* projectDefinition, json data)
        : IDefinition(data),
          mProjectDefinition(projectDefinition)
    {
        setLogClassName("SceneDefinition");
        auto log = getLog();
        log->trace( "Constructing {}", getNameAndUuidString() );
    }

    SceneDefinition::~SceneDefinition
    ()
    {
        auto log = getLog();
        log->trace( "Destructing {}", getNameAndUuidString() );
        if (mRootSceneObjectDefinition != nullptr)
        {
            delete mRootSceneObjectDefinition;
            mRootSceneObjectDefinition = nullptr;
        }
    }

    void
    SceneDefinition::showStatus
    ()
    {
        auto log = getLog();
        log->debug( mJson.dump(1) );
    }

    void
    SceneDefinition::loadRootSceneObjectDefinition
    ()
    {
        json rsoJson = mJson[Constants::SCENE_ROOT_SCENE_OBJECT];
        auto log = getLog();
        if (rsoJson.is_null())
        {
            log->error( "No root SceneObject found!!" );
            return;
        }

        mRootSceneObjectDefinition = new SceneObjectDefinition
        (
            nullptr,
            this,
            rsoJson
        );
        mRootSceneObjectDefinition->loadChildSceneObjectDefinitions();
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

    string
    SceneDefinition::getNotes
    ()
    {
        if (mJson[Constants::SCENE_NOTES].is_null())
        {
            mJson[Constants::SCENE_NOTES] = "";
        }
        return mJson[Constants::SCENE_NOTES];
    }

    void
    SceneDefinition::setNotes
    (string notes)
    {
        mJson[Constants::SCENE_NOTES] = notes;
    }

    vec3
    SceneDefinition::getCameraTranslation
    ()
    {
        if (mJson[Constants::SCENE_CAMERA_TRANSLATION].is_null())
        {
           setCameraTranslation(vec3(0));
        }
        return vec3(
            mJson[Constants::SCENE_CAMERA_TRANSLATION][Constants::X],
            mJson[Constants::SCENE_CAMERA_TRANSLATION][Constants::Y],
            mJson[Constants::SCENE_CAMERA_TRANSLATION][Constants::Z]
        );
    }

    void
    SceneDefinition::setCameraTranslation
    (vec3 transform)
    {
        if (mJson[Constants::SCENE_CAMERA_TRANSLATION].is_null())
        {
            mJson[Constants::SCENE_CAMERA_TRANSLATION] = json::object();
        }
        // Translation
        mJson[Constants::SCENE_CAMERA_TRANSLATION][Constants::X] = transform.x;
        mJson[Constants::SCENE_CAMERA_TRANSLATION][Constants::Y] = transform.y;
        mJson[Constants::SCENE_CAMERA_TRANSLATION][Constants::Z] = transform.z;
    }

    glm::vec3 SceneDefinition::getCameraLookAt()
    {
        if (mJson[Constants::SCENE_CAMERA_LOOK_AT].is_null())
        {
           setCameraLookAt(vec3(0));
        }
        return vec3(
            mJson[Constants::SCENE_CAMERA_LOOK_AT][Constants::X],
            mJson[Constants::SCENE_CAMERA_LOOK_AT][Constants::Y],
            mJson[Constants::SCENE_CAMERA_LOOK_AT][Constants::Z]
        );
    }

    void SceneDefinition::setCameraLookAt(glm::vec3 lookAt)
    {
        if (mJson[Constants::SCENE_CAMERA_LOOK_AT].is_null())
        {
            mJson[Constants::SCENE_CAMERA_LOOK_AT] = json::object();
        }
        // Translation
        mJson[Constants::SCENE_CAMERA_LOOK_AT][Constants::X] = lookAt.x;
        mJson[Constants::SCENE_CAMERA_LOOK_AT][Constants::Y] = lookAt.y;
        mJson[Constants::SCENE_CAMERA_LOOK_AT][Constants::Z] = lookAt.z;
    }

    vector<float>
    SceneDefinition::getGravity
    ()
    {
        vector<float> gravity;

        if (mJson[Constants::SCENE_GRAVITY].is_null())
        {
            mJson[Constants::SCENE_GRAVITY][Constants::X] = 0.0f;
            mJson[Constants::SCENE_GRAVITY][Constants::Y] = 0.0f;
            mJson[Constants::SCENE_GRAVITY][Constants::Z] = 0.0f;

        }

        gravity.push_back(mJson[Constants::SCENE_GRAVITY][Constants::X]);
        gravity.push_back(mJson[Constants::SCENE_GRAVITY][Constants::Y]);
        gravity.push_back(mJson[Constants::SCENE_GRAVITY][Constants::Z]);

        return gravity;
    }

    void
    SceneDefinition::setGravity
    (vector<float> gravity)
    {
        if (mJson[Constants::SCENE_GRAVITY].is_null())
        {
            mJson[Constants::SCENE_GRAVITY] = {};
        }

        mJson[Constants::SCENE_GRAVITY][Constants::X] = gravity[Constants::X_INDEX];
        mJson[Constants::SCENE_GRAVITY][Constants::Y] = gravity[Constants::Y_INDEX];
        mJson[Constants::SCENE_GRAVITY][Constants::Z] = gravity[Constants::Z_INDEX] ;
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

    vector<float>
    SceneDefinition::getClearColour
    ()
    {
        vector<float> colour;

        if (mJson[Constants::SCENE_CLEAR_COLOUR].is_null())
        {
            mJson[Constants::SCENE_CLEAR_COLOUR][Constants::RED]   = 0.0f;
            mJson[Constants::SCENE_CLEAR_COLOUR][Constants::GREEN] = 0.0f;
            mJson[Constants::SCENE_CLEAR_COLOUR][Constants::BLUE]  = 0.0f;

        }

        colour.push_back(mJson[Constants::SCENE_CLEAR_COLOUR][Constants::RED]);
        colour.push_back(mJson[Constants::SCENE_CLEAR_COLOUR][Constants::GREEN]);
        colour.push_back(mJson[Constants::SCENE_CLEAR_COLOUR][Constants::BLUE]);

        return colour;
    }

    void
    SceneDefinition::setClearColour
    (vector<float> colour)
    {
        if (mJson[Constants::SCENE_CLEAR_COLOUR].is_null())
        {
            mJson[Constants::SCENE_CLEAR_COLOUR] = {};
        }

        mJson[Constants::SCENE_CLEAR_COLOUR][Constants::RED]   = colour[Constants::RED_INDEX];
        mJson[Constants::SCENE_CLEAR_COLOUR][Constants::GREEN] = colour[Constants::GREEN_INDEX];
        mJson[Constants::SCENE_CLEAR_COLOUR][Constants::BLUE]  = colour[Constants::BLUE_INDEX] ;
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

    vector<float>
    SceneDefinition::getAmbientColour
    ()
    {
        vector<float> colour;

        if (mJson[Constants::SCENE_AMBIENT_LIGHT_COLOUR].is_null() ||
            mJson[Constants::SCENE_AMBIENT_LIGHT_COLOUR][Constants::RED].is_null())
        {
            mJson[Constants::SCENE_AMBIENT_LIGHT_COLOUR] = {};
            mJson[Constants::SCENE_AMBIENT_LIGHT_COLOUR][Constants::RED] = 1.0f;
            mJson[Constants::SCENE_AMBIENT_LIGHT_COLOUR][Constants::GREEN] = 1.0f;
            mJson[Constants::SCENE_AMBIENT_LIGHT_COLOUR][Constants::BLUE] = 1.0f;
            mJson[Constants::SCENE_AMBIENT_LIGHT_COLOUR][Constants::ALPHA] = 1.0f;
        }

        colour.push_back(mJson[Constants::SCENE_AMBIENT_LIGHT_COLOUR][Constants::RED]);
        colour.push_back(mJson[Constants::SCENE_AMBIENT_LIGHT_COLOUR][Constants::GREEN]);
        colour.push_back(mJson[Constants::SCENE_AMBIENT_LIGHT_COLOUR][Constants::BLUE]);
        colour.push_back(mJson[Constants::SCENE_AMBIENT_LIGHT_COLOUR][Constants::ALPHA]);

        return colour;
    }

    void
    SceneDefinition::setAmbientColour
    (vector<float> colour)
    {
        if (mJson[Constants::SCENE_CLEAR_COLOUR].is_null())
        {
            mJson[Constants::SCENE_CLEAR_COLOUR] = {};
        }

        mJson[Constants::SCENE_AMBIENT_LIGHT_COLOUR][Constants::RED]   = colour[Constants::RED_INDEX];
        mJson[Constants::SCENE_AMBIENT_LIGHT_COLOUR][Constants::GREEN] = colour[Constants::GREEN_INDEX];
        mJson[Constants::SCENE_AMBIENT_LIGHT_COLOUR][Constants::BLUE]  = colour[Constants::BLUE_INDEX] ;
        mJson[Constants::SCENE_AMBIENT_LIGHT_COLOUR][Constants::ALPHA] = colour[Constants::ALPHA_INDEX];
    }

    void
    SceneDefinition::setAmbientColourR
    (float r)
    {

        mJson[Constants::SCENE_AMBIENT_LIGHT_COLOUR][Constants::RED] = r;
    }

    void
    SceneDefinition::setAmbientColourG
    (float g)
    {

        mJson[Constants::SCENE_AMBIENT_LIGHT_COLOUR][Constants::GREEN] =g;
    }

    void
    SceneDefinition::setAmbientColourB
    (float b)
    {

        mJson[Constants::SCENE_AMBIENT_LIGHT_COLOUR][Constants::BLUE] = b;
    }

    void
    SceneDefinition::setAmbientColourA
    (float a)
    {

        mJson[Constants::SCENE_AMBIENT_LIGHT_COLOUR][Constants::ALPHA] = a;
    }

    SceneObjectDefinition*
    SceneDefinition::getRootSceneObjectDefinition
    ()
    {
        return mRootSceneObjectDefinition;
    }

    ProjectDefinition*
    SceneDefinition::getProjectDefinition
    ()
    {
        return mProjectDefinition;
    }

    SceneObjectDefinition*
    SceneDefinition::createNewRootSceneObjectDefinition
    ()
    {
        json rootDefJson;
        rootDefJson[Constants::NAME] = Constants::SCENE_OBJECT_ROOT_NAME;
        rootDefJson[Constants::UUID] = Uuid::generateUuid();
        Transform transform;
        rootDefJson[Constants::TRANSFORM] = transform.getJson();
        mRootSceneObjectDefinition = new SceneObjectDefinition
        (
            nullptr,
            this,
            rootDefJson
        );
        return mRootSceneObjectDefinition;
    }

    json
    SceneDefinition::getJson
    ()
    {
        mJson[Constants::SCENE_ROOT_SCENE_OBJECT] = mRootSceneObjectDefinition->getJson();
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


    float SceneDefinition::getCameraLookAtX()
    {
        if (mJson[Constants::SCENE_CAMERA_LOOK_AT].is_null())
        {
           setCameraLookAt(vec3(0));
        }
        return mJson[Constants::SCENE_CAMERA_LOOK_AT][Constants::X];
    }

    float SceneDefinition::getCameraLookAtY()
    {
        if (mJson[Constants::SCENE_CAMERA_LOOK_AT].is_null())
        {
           setCameraLookAt(vec3(0));
        }
        return mJson[Constants::SCENE_CAMERA_LOOK_AT][Constants::Y];
    }

    float SceneDefinition::getCameraLookAtZ()
    {
        if (mJson[Constants::SCENE_CAMERA_LOOK_AT].is_null())
        {
           setCameraLookAt(vec3(0));
        }
        return mJson[Constants::SCENE_CAMERA_LOOK_AT][Constants::Z];
    }

    void SceneDefinition::setCameraLookAtX(float val)
    {
        if (mJson[Constants::SCENE_CAMERA_LOOK_AT].is_null())
        {
           setCameraLookAt(vec3(0));
        }
        mJson[Constants::SCENE_CAMERA_LOOK_AT][Constants::X] = val;
    }

    void SceneDefinition::setCameraLookAtY(float val)
    {
        if (mJson[Constants::SCENE_CAMERA_LOOK_AT].is_null())
        {
           setCameraLookAt(vec3(0));
        }
        mJson[Constants::SCENE_CAMERA_LOOK_AT][Constants::Y] = val;
    }

    void SceneDefinition::setCameraLookAtZ(float val)
    {
        if (mJson[Constants::SCENE_CAMERA_LOOK_AT].is_null())
        {
           setCameraLookAt(vec3(0));
        }
        mJson[Constants::SCENE_CAMERA_LOOK_AT][Constants::Z] = val;
    }

    float SceneDefinition::getCameraTranslationX()
    {
        if (mJson[Constants::SCENE_CAMERA_TRANSLATION].is_null())
        {
           setCameraTranslation(vec3(0));
        }
        return mJson[Constants::SCENE_CAMERA_TRANSLATION][Constants::X];
    }

    float SceneDefinition::getCameraTranslationY()
    {
        if (mJson[Constants::SCENE_CAMERA_TRANSLATION].is_null())
        {
           setCameraTranslation(vec3(0));
        }
        return mJson[Constants::SCENE_CAMERA_TRANSLATION][Constants::Y];
    }

    float SceneDefinition::getCameraTranslationZ()
    {
        if (mJson[Constants::SCENE_CAMERA_TRANSLATION].is_null())
        {
           setCameraTranslation(vec3(0));
        }
        return mJson[Constants::SCENE_CAMERA_TRANSLATION][Constants::Z];
    }

    void SceneDefinition::setCameraTranslationX(float val)
    {
        if (mJson[Constants::SCENE_CAMERA_TRANSLATION].is_null())
        {
           setCameraTranslation(vec3(0));
        }
        mJson[Constants::SCENE_CAMERA_TRANSLATION][Constants::X] = val;
    }

    void SceneDefinition::setCameraTranslationY(float val)
    {
        if (mJson[Constants::SCENE_CAMERA_TRANSLATION].is_null())
        {
           setCameraTranslation(vec3(0));
        }
        mJson[Constants::SCENE_CAMERA_TRANSLATION][Constants::Y] = val;
    }

    void SceneDefinition::setCameraTranslationZ(float val)
    {
        if (mJson[Constants::SCENE_CAMERA_TRANSLATION].is_null())
        {
           setCameraTranslation(vec3(0));
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

    string
    SceneDefinition::getLightingShader
    ()
    {
       if (mJson[Constants::SCENE_LIGHTING_SHADER].is_null())
       {
           mJson[Constants::SCENE_LIGHTING_SHADER] = "";
       }
       return  mJson[Constants::SCENE_LIGHTING_SHADER];

    }

    void
    SceneDefinition::setLightingShader
    (string shader)
    {
       mJson[Constants::SCENE_LIGHTING_SHADER] = shader;
    }

    int
    SceneDefinition::getCurrentLightingShaderIndex
    ()
    {
        return mProjectDefinition->getAssetDefinitionIndex(AssetType::SHADER, getLightingShader());
    }
}
