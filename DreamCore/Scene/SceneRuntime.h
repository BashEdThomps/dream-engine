/*
 * SceneRuntime.h
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

#pragma once

#include <string>
#include <vector>

#include "SceneState.h"

#include "../Common/IRuntime.h"

using std::string;
using std::vector;
using std::unique_ptr;

namespace Dream
{
    class ProjectRuntime;
    class SceneObject;
    class SceneDefinition;
    class IAssetInstance;
    class Transform3D;
    class AssetDefinition;
    class SceneObjectRuntime;


    class SceneRuntime : public Runtime
    {
    private:
        SceneState mState;
        vector<float> mGravity;
        vector<float> mClearColour;
        vector<float> mAmbientColour;
        ProjectRuntime* mProjectRuntimeHandle;

        vector<SceneObjectRuntime*> mSceneObjectRuntimeDeleteQueue;
        unique_ptr<SceneObjectRuntime> mRootSceneObjectRuntime;

    public:
        SceneRuntime(ProjectRuntime* parent = nullptr);
        ~SceneRuntime();

        SceneState getState() const;
        void setState(const SceneState &state);

        vector<float> getGravity() const;
        void setGravity(const vector<float> &gravity);

        vector<float> getClearColour() const;
        void setClearColour(const vector<float> &clearColour);

        vector<float> getAmbientColour() const;
        void setAmbientColour(const vector<float> &ambientColour);

        vector<SceneObjectRuntime*> getSceneObjectRuntimeDeleteQueue();
        void addSceneObjectRuntimeToDeleteQueue(SceneObjectRuntime*);
        void clearSceneObjectRuntimeDeleteQueue();
        void destroySceneObjectRuntimeDeleteQueue();

        void findDeleteFlaggedSceneObjectRuntimes();
        void findDeleteFlaggedScripts();

        void createAllAssetInstances();
        void loadAllAssetInstances();

        void cleanUpSceneObjectRuntimes();
        void cleanUp();

        bool hasRootSceneObjectRuntime();
        void setRootSceneObjectRuntime(SceneObjectRuntime* sceneObjectHandle);
        SceneObjectRuntime* getRootSceneObjectRuntimeHandle();

        SceneObjectRuntime* getSceneObjectRuntimeHandleByName(string);
        SceneObjectRuntime* getSceneObjectRuntimeHandleByUuid(string);

        int countSceneObjectRuntimes();

        ProjectRuntime* getProjectRuntimeHandle();

        void showScenegraph();

        int countChildrenOfSceneObjectRuntime(SceneObjectRuntime*);

        void flush();
    };
} // End of Dream
