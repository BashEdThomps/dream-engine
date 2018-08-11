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

#include "../Common/Constants.h"
#include "../Components/Transform3D.h"

#include "../Project/ProjectDefinition.h"

#include "SceneObject/SceneObjectDefinition.h"

#include "../Utilities/String.h"
#include "../Utilities/Uuid.h"

using std::cout;
using std::endl;

namespace Dream
{
    SceneDefinition::SceneDefinition
    (ProjectDefinition* projectDefinitionHandle, json data)
        : IDefinition(data),
          ILoggable("SceneDefinition"),
          mProjectDefinitionHandle(projectDefinitionHandle)
    {
        auto log = getLog();
        log->trace( "Constructing ", getNameAndUuidString() );

        json rootSceneObject = mJson[Constants::SCENE_ROOT_SCENE_OBJECT];
        mCameraTransform = Transform3D(mJson[Constants::SCENE_CAMERA_TRANSFORM]);

        if (rootSceneObject.is_null())
        {
            log->info( "No root SceneObject found!!" );
        }
        else
        {
            loadRootSceneObjectDefinition(rootSceneObject);
        }
    }

    SceneDefinition::~SceneDefinition
    ()
    {
        auto log = getLog();
        log->trace( "Destructing {}", getNameAndUuidString() );
    }

    void
    SceneDefinition::showStatus
    ()
    {
        auto log = getLog();
        log->info( mJson.dump(1) );
    }

    void
    SceneDefinition::loadRootSceneObjectDefinition
    (json rsoJson)
    {
        mRootSceneObjectDefinition.reset(new SceneObjectDefinition(nullptr, this, rsoJson));
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

    Transform3D&
    SceneDefinition::getCameraTransform
    ()
    {
        return mCameraTransform;
    }

    void
    SceneDefinition::setCameraTransform
    (Transform3D transform)
    {
        mCameraTransform = transform;
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
    SceneDefinition::getRootSceneObjectDefinitionHandle
    ()
    {
        return mRootSceneObjectDefinition.get();
    }

    ProjectDefinition*
    SceneDefinition::getProjectDefinitionHandle
    ()
    {
        return mProjectDefinitionHandle;
    }

    SceneObjectDefinition*
    SceneDefinition::createNewRootSceneObjectDefinition
    ()
    {
        json rootDefJson;
        rootDefJson[Constants::NAME] = Constants::SCENE_OBJECT_ROOT_NAME;
        rootDefJson[Constants::UUID] = Uuid::generateUuid();
        Transform3D transform;
        rootDefJson[Constants::TRANSFORM] = transform.getJson();
        SceneObjectDefinition *rootSoDefinition;
        rootSoDefinition = new SceneObjectDefinition(nullptr,this,rootDefJson);
        mRootSceneObjectDefinition.reset(rootSoDefinition);
        return rootSoDefinition;
    }

    json
    SceneDefinition::getJson
    ()
    {
        mJson[Constants::SCENE_ROOT_SCENE_OBJECT] = mRootSceneObjectDefinition->getJson();
        mJson[Constants::SCENE_CAMERA_TRANSFORM] = getCameraTransform().getJson();
        return mJson;
    }
}
