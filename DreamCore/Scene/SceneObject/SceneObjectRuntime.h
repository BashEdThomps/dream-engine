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

#include <vector>

#include <glm/vec3.hpp>
#include "BoundingBox.h"
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
    class PhysicsObjectInstance;
    class ScriptInstance;
    class ParticleEmitterInstance;
    class SceneRuntime;
    class SceneObjectDefinition;
    class IAssetDefinition;
    class PathDefinition;
    class AudioDefinition;
    class LightDefinition;
    class ModelDefinition;
    class PhysicsObjectDefinition;
    class ScriptDefinition;
    class ParticleEmitterDefinition;
    class IAssetInstance;

    class SceneObjectRuntime : public IRuntime
    {
    public:
        SceneObjectRuntime(SceneObjectDefinition* sd, SceneRuntime* sceneRuntime = nullptr);
        ~SceneObjectRuntime() override;

        void collectGarbage() override;

        SceneRuntime* getSceneRuntime();
        SceneObjectDefinition* getSceneObjectDefinition();

        bool createAssetInstances();
        bool createPathInstance(PathDefinition*);
        bool createAudioInstance(AudioDefinition*);
        bool createModelInstance(ModelDefinition*);
        bool createScriptInstance(ScriptDefinition*);
        bool createPhysicsObjectInstance(PhysicsObjectDefinition*);
        bool createParticleEmitterInstance(ParticleEmitterDefinition*);
        bool createLightInstance(LightDefinition*);

        PathInstance*  getPathInstance();
        AudioInstance* getAudioInstance();
        ModelInstance* getModelInstance();
        ScriptInstance* getScriptInstance();
        PhysicsObjectInstance* getPhysicsObjectInstance();
        LightInstance* getLightInstance();
        ParticleEmitterInstance* getParticleEmitterInstance();
        IAssetInstance* getAssetInstance(AssetType);

        bool hasPathInstance();
        bool hasAudioInstance();
        bool hasModelInstance();
        bool hasScriptInstance();
        bool hasPhysicsObjectInstance();
        bool hasLightInstance();

        TransformType getTransformType() const;
        void setTransformType(TransformType);

        Transform3D* getCurrentTransform();
        Transform3D* getDefinedTransform();

        bool hasFocus() const;
        void setHasFocus(bool);

        bool hasEvents() const;
        void addEvent(Event);
        const vector<Event>& getEventQueue() const;
        void clearEventQueue();

        SceneObjectRuntime* getChildRuntimeByUuid(string);

        int countAllChildren();
        size_t countChildren();
        void addChildRuntime(SceneObjectRuntime*);
        void removeChildRuntime(SceneObjectRuntime*);
        SceneObjectRuntime* createChildRuntime(SceneObjectDefinition*);
        vector<SceneObjectRuntime*> getChildRuntimes();
        bool isChildOf(SceneObjectRuntime*);

        bool isParentOf(SceneObjectRuntime* child);
        void setParentRuntime(SceneObjectRuntime* parent);
        SceneObjectRuntime* getParentRuntime();
        SceneObjectRuntime* applyToAll(function<SceneObjectRuntime*(SceneObjectRuntime*)>);
        bool applyToAll(function<bool(SceneObjectRuntime*)>);

        bool useDefinition() override;

        bool getFollowsCamera() const;
        void setFollowsCamera(bool followsCamera);

        bool getDeleted() const;
        void setDeleted(bool deleted);

        bool getHidden() const;
        void setHidden(bool hidden);

        void removeAudioInstance();
        void removePathInstance();
        void removeModelInstance();
        void removeLightInstance();
        void removeScriptInstance();
        void removePhysicsObjectInstance();
        void removeParticleEmitterInstance();

        bool replaceAssetUuid(AssetType type, string uuid);
        IAssetDefinition*getAssetDefinitionByUuid(string uuid);
        string getProjectPath();
        void setAssetDefinitionsMap(map<AssetType,string> loadQueue);
        map<AssetType, string> getAssetDefinitionsMap();
        bool getAlwaysDraw() const;
        void setAlwaysDraw(bool alwaysDraw);

        BoundingBox& getBoundingBox();
        void setBoundingBox(BoundingBox boundingBox);

    protected:
        void initTransform();
        bool loadChildrenFromDefinition(SceneObjectDefinition* definition);

    private:
        AudioInstance* mAudioInstance;
        LightInstance* mLightInstance;
        ParticleEmitterInstance* mParticleEmitterInstance;
        PathInstance* mPathInstance;
        PhysicsObjectInstance* mPhysicsObjectInstance;
        ScriptInstance* mScriptInstance;
        ModelInstance* mModelInstance;
        Transform3D* mCurrentTransform;
        Transform3D* mDefinedTransform;
        vector<Event> mEventQueue;
        map<AssetType,string> mAssetDefinitions;
        vector<SceneObjectRuntime*> mChildRuntimes;
        SceneRuntime* mSceneRuntimeHandle;
        SceneObjectRuntime* mParentRuntimeHandle;
        BoundingBox mBoundingBox;
        bool mHasFocus;
        bool mDeleted;
        bool mHidden;
        bool mAlwaysDraw;
        bool mFollowsCamera;
    };
}
