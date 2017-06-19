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

using std::cout;
using std::endl;

namespace Dream
{
    SceneDefinition::SceneDefinition(ProjectDefinition* projectDefinitionHandle, json data)
        : IDefinition(data),
          mProjectDefinitionHandle(projectDefinitionHandle)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneDefinition: Constructing "
                 << getNameAndUuidString() << endl;
        }

        json sceneObjects = mJson[Constants::SCENE_SCENE_OBJECTS];

        if (!sceneObjects.is_null() && sceneObjects.is_array())
        {
            loadRootSceneObjectDefinition(sceneObjects[0]);
        }
    }

    SceneDefinition::~SceneDefinition()
    {
        if (Constants::DEBUG)
        {
            cout << "SceneDefinition: Destructing "
                 << getNameAndUuidString() << endl;
        }
    }

    void
    SceneDefinition::showStatus
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "SceneDefinition: " << mJson.dump(1) << endl;
        }
    }

    void
    SceneDefinition::loadRootSceneObjectDefinition
    (json jsonArray)
    {
        if (!jsonArray.is_null())
        {
            mRootSceneObjectDefinition.reset(new SceneObjectDefinition(nullptr, jsonArray));
        }
    }

    void
    SceneDefinition::setCameraMovementSpeed
    (float speed)
    {
        mJson[Constants::SCENE_CAMERA][Constants::SCENE_MOVEMENT_SPEED] = speed;
    }

    float
    SceneDefinition::getCameraMovementSpeed
    ()
    {
        return mJson[Constants::SCENE_CAMERA][Constants::SCENE_MOVEMENT_SPEED];
    }

    bool
    SceneDefinition::getPhysicsDebug
    ()
    {
        if (mJson[Constants::SCENE_PHYSICS_DEBUG].is_null())
        {
            return false;
        }
        return mJson[Constants::SCENE_PHYSICS_DEBUG];
    }

    string
    SceneDefinition::getNotes
    ()
    {
        if (mJson[Constants::SCENE_NOTES].is_null())
        {
            return "";
        }
        return mJson[Constants::SCENE_NOTES];
    }

    void
    SceneDefinition::setNotes
    (string notes)
    {
        mJson[Constants::SCENE_NOTES] = notes;
    }

    Transform3D
    SceneDefinition::getCameraTransform
    ()
    {
        Transform3D defaultCameraTransform;

        json camera = mJson[Constants::SCENE_CAMERA];

        if (!camera.is_null())
        {
            json translation = camera[Constants::SCENE_TRANSLATION];
            defaultCameraTransform.setTranslation(
                translation[Constants::X],
                translation[Constants::Y],
                translation[Constants::Z]
            );
            json rotation = camera[Constants::SCENE_ROTATION];
            defaultCameraTransform.setRotation(
                rotation[Constants::X],
                rotation[Constants::Y],
                rotation[Constants::Z]
            );
        }

        return defaultCameraTransform;
    }

    vector<float>
    SceneDefinition::getGravity
    ()
    {
        vector<float> gravity;

        if (mJson[Constants::SCENE_GRAVITY].is_null())
        {
            if (Constants::DEBUG)
            {
                cout << "SceneDefinition: Scene gravity is null" << endl;
            }
            return gravity;
        }

        if (Constants::DEBUG)
        {
            cout <<"SceneDefinition: Gravity is " << mJson[Constants::SCENE_GRAVITY] << endl;
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

    vector<float>
    SceneDefinition::getClearColour
    ()
    {
        vector<float> colour;

        if (mJson[Constants::SCENE_CLEAR_COLOUR].is_null())
        {
            if (Constants::DEBUG)
            {
                cout << "SceneDefinition: Could not find clear colour in json" << endl;
            }
            return colour;
        }

        colour.push_back(mJson[Constants::SCENE_CLEAR_COLOUR][Constants::RED]);
        colour.push_back(mJson[Constants::SCENE_CLEAR_COLOUR][Constants::GREEN]);
        colour.push_back(mJson[Constants::SCENE_CLEAR_COLOUR][Constants::BLUE]);

        if (Constants::DEBUG)
        {
            cout << "SceneObjectDefinition: Using clear colour "
                 << String::floatVectorToString(colour) << endl;
        }

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

    vector<float>
    SceneDefinition::getAmbientColour
    ()
    {
        vector<float> colour;

        if (mJson[Constants::SCENE_CLEAR_COLOUR].is_null())
        {
            return colour;
        }

        colour.push_back(mJson[Constants::SCENE_CLEAR_COLOUR][Constants::RED]);
        colour.push_back(mJson[Constants::SCENE_CLEAR_COLOUR][Constants::GREEN]);
        colour.push_back(mJson[Constants::SCENE_CLEAR_COLOUR][Constants::BLUE]);
        colour.push_back(mJson[Constants::SCENE_CLEAR_COLOUR][Constants::ALPHA]);

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

    SceneObjectDefinition*
    SceneDefinition::getRootSceneObjectDefinitionHandle
    ()
    {
        return mRootSceneObjectDefinition.get();
    }
}
