/*
 * SceneRuntime.cpp
 *
 * Created: 15 2017 by Ashley
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

#include "SceneRuntime.h"

#include <iostream>

#include "../Common/Constants.h"

#include "SceneObject/SceneObject.h"
#include "SceneObject/SceneObjectDefinition.h"
#include "SceneObject/SceneObjectRuntime.h"

using std::cout;
using std::cerr;

namespace Dream
{
    SceneRuntime::SceneRuntime
    ()
        : // Init list
          Runtime(),
          mGravity({0,0,0}),
          mClearColour({0,0,0,0}),
          mAmbientColour({0,0,0})
    {
        if (Constants::DEBUG)
        {
            cout << "SceneRuntime: Constructing Object" << endl;
        }

    }

    SceneRuntime::~SceneRuntime
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "SceneRuntime: Destroying Object" << endl;
        }
    }

    SceneState
    SceneRuntime::getState
    ()
    const
    {
        return mState;
    }

    void
    SceneRuntime::setState
    (const SceneState &state)
    {
        mState = state;
    }

    vector<float>
    SceneRuntime::getGravity
    ()
    const
    {
        return mGravity;
    }

    void
    SceneRuntime::setGravity
    (const vector<float> &gravity)
    {
        mGravity = gravity;
    }

    vector<float>
    SceneRuntime::getClearColour
    ()
    const
    {
        return mClearColour;
    }

    void
    SceneRuntime::setClearColour
    (const vector<float> &clearColour)
    {
        mClearColour = clearColour;
    }

    vector<float>
    SceneRuntime::getAmbientColour
    () const
    {
        return mAmbientColour;
    }

    void
    SceneRuntime::setAmbientColour
    (const vector<float> &ambientColour)
    {
        mAmbientColour = ambientColour;
    }

    string
    SceneRuntime::getProjectPath
    () const
    {
        return mProjectPath;
    }

    void
    SceneRuntime::setProjectPath
    (const string &projectPath)
    {
        mProjectPath = projectPath;
    }

    vector<SceneObject*>
    SceneRuntime::getDeleteQueue
    () const
    {
        return mDeleteQueue;
    }

    void
    SceneRuntime::addToDeleteQueue
    (SceneObject* object)
    {
        if (Constants::DEBUG)
        {
            cout << "Scene: Adding "
                 << object->getDefinitionHandle()->getNameAndUuidString()
                 << " to delete queue" << endl;
        }
        getDeleteQueue().push_back(object);
    }

    void
    SceneRuntime::clearDeleteQueue
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "Scene: clearDeleteQueue" << endl;
        }
        getDeleteQueue().clear();
    }

    void
    SceneRuntime::destroyDeleteQueue
    ()
    {
        if (!mDeleteQueue.empty())
        {
            for(SceneObject* obj : mDeleteQueue)
            {
                obj->getRuntimeHandle()->cleanUp();
            }
        }
        clearDeleteQueue();
    }

} // End of Dream
