/*
 * SceneDefinition.h
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

#pragma once

#include <vector>
#include <memory>
#include "../Common/IDefinition.h"

using std::vector;
using std::unique_ptr;

namespace Dream
{
    class ProjectDefinition;
    class SceneObject;
    class Transform3D;
    class SceneObjectDefinition;

    class SceneDefinition : public IDefinition
    {

    private:
        unique_ptr<SceneObjectDefinition> mRootSceneObjectDefinition;
        ProjectDefinition* mProjectDefinitionHandle;

    public:
        SceneDefinition(ProjectDefinition* projectHandle, json data);
        ~SceneDefinition();

        void showStatus();

        void loadRootSceneObjectDefinition(json jsonArray);

        void setCameraMovementSpeed(float speed);
        float getCameraMovementSpeed();

        bool getPhysicsDebug();

        string getNotes();
        void setNotes(string notes);

        Transform3D getCameraTransform();

        vector<float> getGravity();
        void setGravity(vector<float> gravity);

        vector<float> getClearColour();
        void setClearColour(vector<float> clearColour);

        vector<float> getAmbientColour();
        void setAmbientColour(vector<float> ambientColour);

        SceneObjectDefinition* getRootSceneObjectDefinitionHandle();

        ProjectDefinition* getProjectDefinitionHandle();
    };
}
