/*
 * SceneObjectRuntime.h
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

#include <memory>
#include <vector>
#include <glm/vec3.hpp>

#include "../../Common/IRuntime.h"

#include "../../Components/Transform3D.h"

using std::vector;
using std::function;

using nlohmann::json;
using glm::vec3;

namespace Dream
{
    class SceneObject;
    class Scene;
    class Event;
    class AudioInstance;
    class PathInstance;
    class ModelInstance;
    class LightInstance;
    class ShaderInstance;
    class PhysicsObjectInstance;
    class ScriptInstance;
    class SceneRuntime;
    class SceneObjectDefinition;

    class IAssetDefinition;
    class PathDefinition;
    class AudioDefinition;
    class LightDefinition;
    class ModelDefinition;
    class PhysicsObjectDefinition;
    class ScriptDefinition;
    class ShaderDefinition;

    class SceneObjectRuntime : public IRuntime
    {
    public:
        SceneObjectRuntime(SceneObjectDefinition* sd, SceneRuntime* sceneRuntime = nullptr);
        ~SceneObjectRuntime() override;

        void collectGarbage() override;

        SceneRuntime* getSceneRuntime();
        SceneObjectDefinition* getSceneObjectDefinition();

        bool createAssetInstances();
        bool createAssetInstanceFromAssetDefinitionByUuid(string);
        bool createAssetInstance        (IAssetDefinition*);
        bool createPathInstance         (PathDefinition*);
        bool createAudioInstance        (AudioDefinition*);
        bool createModelInstance        (ModelDefinition*);
        bool createScriptInstance       (ScriptDefinition*);
        bool createShaderInstance       (ShaderDefinition*);
        bool createPhysicsObjectInstance(PhysicsObjectDefinition*);
        bool createLightInstance        (LightDefinition*);

        quat getOrientation();

        vec3 getTranslation();
        void setTranslation(float, float, float);
        void setTranslation(vec3);
        void resetTranslation();

        glm::vec3 getRotation();
        void setRotation(float, float, float);
        void setRotation(glm::vec3);
        void resetRotation();

        glm::vec3 getScale();
        void setScale(float, float, float);
        void setScale(glm::vec3);
        void resetScale();

        void resetTransform();

        PathInstance*  getPathInstance();
        AudioInstance* getAudioInstance();
        ModelInstance* getModelInstance();
        ScriptInstance* getScriptInstance();
        ShaderInstance* getShaderInstance();
        PhysicsObjectInstance* getPhysicsObjectInstance();
        LightInstance* getLightInstance();

        bool hasPathInstance();
        bool hasAudioInstance();
        bool hasModelInstance();
        bool hasScriptInstance();
        bool hasShaderInstance();
        bool hasPhysicsObjectInstance();
        bool hasLightInstance();

        void addAssetDefinitionUuidToLoad(string);
        vector<string> getAssetDefinitionUuidsToLoad();

        string getTransformType() const;
        void setTransformType(string);

        Transform3D* getTransform();
        void setTransform(Transform3D*);

        bool hasFocus() const;
        void setHasFocus(bool);

        bool getLoadedFlag() const;
        void setLoadedFlag(bool);

        bool hasEvents() const;
        void addEvent(Event);
        const vector<Event>& getEventQueue() const;
        void clearEventQueue();

        SceneObjectRuntime* getChildRuntimeByUuid(string);

        int countAllChildren();
        size_t countChildren();
        void addChildRuntime(SceneObjectRuntime*);
        void removeChildRuntime(SceneObjectRuntime*);
        vector<SceneObjectRuntime*> getChildRuntimes();
        bool isChildOf(SceneObjectRuntime*);

        bool isParentOf(SceneObjectRuntime* child);
        void setParentRuntime(SceneObjectRuntime* parent);
        SceneObjectRuntime* getParentRuntime();
        SceneObjectRuntime* applyToAll(function<SceneObjectRuntime*(SceneObjectRuntime*)>);
        bool applyToAll(function<bool(SceneObjectRuntime*)>);

        bool useDefinition() override;

        bool followsCamera() const;
        void setFollowsCamera(bool followsCamera);
        void walk(float,float);
        void drive(float,float);

        bool getDeleted() const;
        void setDeleted(bool deleted);

        bool getHidden() const;
        void setHidden(bool hidden);

        void removeAudioInstance();
        void removePathInstance();
        void removeModelInstance();
        //void removeShaderInstance();
        void removeLightInstance();
        void removeScriptInstance();
        void removePhysicsObjectInstance();

        bool replaceAssetUuid(string uuid);
        IAssetDefinition*getAssetDefinitionByUuid(string uuid);
        string getProjectPath();
    private:

        AudioInstance* mAudioInstance;
        PathInstance* mPathInstance;
        LightInstance* mLightInstance;
        ScriptInstance* mScriptInstance;
        PhysicsObjectInstance* mPhysicsObjectInstance;
        Transform3D* mTransform;

        ModelInstance* mModelInstance;
        ShaderInstance* mShaderInstance;

        vector<Event> mEventQueue;
        vector<string> mAssetDefinitionUuidLoadQueue;
        vector<SceneObjectRuntime*> mChildRuntimes;

        SceneRuntime* mSceneRuntimeHandle;
        SceneObjectRuntime* mParentRuntimeHandle;
        string mProjectPath;

        bool mLoaded;
        bool mHasFocus;
        bool mDeleted;
        bool mHidden;

        void setAssetDefinitionLoadQueue(vector<string> loadQueue);
        bool loadChildrenFromDefinition(SceneObjectDefinition* definition);
        bool mFollowsCamera;

        void initialTransform();
    };
}
