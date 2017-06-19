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
#include "../Components/Transform3D.h"

using std::string;
using std::vector;
using std::unique_ptr;

namespace Dream
{
    class ProjectRuntime;
    class SceneObject;
    class SceneDefinition;
    class IAssetInstance;
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

        Transform3D mCameraTransform;
        float mCameraMovementSpeed;
        bool mPhysicsDebug;

    public:
        SceneRuntime(ProjectRuntime* parent = nullptr);
        ~SceneRuntime();

        SceneState getState();
        void setState(SceneState state);

        vector<float> getGravity();
        void setGravity(vector<float> gravity);

        vector<float> getClearColour();
        void setClearColour(vector<float> clearColour);

        vector<float> getAmbientColour();
        void setAmbientColour(vector<float> ambientColour);

        vector<SceneObjectRuntime*> getSceneObjectRuntimeDeleteQueue();
        void addSceneObjectRuntimeToDeleteQueue(SceneObjectRuntime*);
        void clearSceneObjectRuntimeDeleteQueue();
        void destroySceneObjectRuntimeDeleteQueue();

        void findDeleteFlaggedSceneObjectRuntimes();
        void findDeleteFlaggedScripts();

        void createAllAssetInstances();
        void loadAllAssetInstances();
        void useDefinition(IDefinition* sceneDefinitionHandle);

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

        void setAssetDefinitionUuidLoadQueue(vector<string> loadQueue);

        bool getPhysicsDebug();
        void setPhysicsDebug(bool physicsDebug);

        float getCameraMovementSpeed();
        void setCameraMovementSpeed(float cameraMovementSpeed);

        Transform3D getCameraTransform();
        void setCameraTransform(Transform3D cameraTransform);

    };
} // End of Dream