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
    class AssimpModelInstance;
    class LightInstance;
    class ShaderInstance;
    class SpriteInstance;
    class FontInstance;
    class PhysicsObjectInstance;
    class ScriptInstance;
    class SceneRuntime;
    class SceneObjectDefinition;

    class IAssetDefinition;
    class PathDefinition;
    class AudioDefinition;
    class LightDefinition;
    class FontDefinition;
    class ModelDefinition;
    class PhysicsObjectDefinition;
    class ScriptDefinition;
    class ShaderDefinition;
    class SpriteDefinition;

    class SceneObjectRuntime : public IRuntime
    {
    public:
        SceneObjectRuntime(SceneObjectDefinition* sd, SceneRuntime* sceneRuntime = nullptr);
        ~SceneObjectRuntime() override;

        void collectGarbage() override;

        SceneRuntime* getSceneRuntime();
        SceneObjectDefinition* getSceneObjectDefinition();

        void createAssetInstances();
        void createAssetInstanceFromAssetDefinitionByUuid(string);
        void createAssetInstance        (IAssetDefinition*);
        void createPathInstance         (PathDefinition*);
        void createAudioInstance        (AudioDefinition*);
        void createModelInstance        (ModelDefinition*);
        void createScriptInstance       (ScriptDefinition*);
        void createShaderInstance       (ShaderDefinition*);
        void createPhysicsObjectInstance(PhysicsObjectDefinition*);
        void createLightInstance        (LightDefinition*);
        void createFontInstance         (FontDefinition*);
        void createSpriteInstance       (SpriteDefinition*);

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
        AssimpModelInstance* getModelInstance();
        ScriptInstance* getScriptInstance();
        ShaderInstance* getShaderInstance();
        PhysicsObjectInstance* getPhysicsObjectInstance();
        LightInstance* getLightInstance();
        SpriteInstance* getSpriteInstance();
        FontInstance* getFontInstance();

        bool hasPathInstance();
        bool hasAudioInstance();
        bool hasModelInstance();
        bool hasScriptInstance();
        bool hasShaderInstance();
        bool hasPhysicsObjectInstance();
        bool hasLightInstance();
        bool hasSpriteInstance();
        bool hasFontInstance();

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
        vector<Event> getEventQueue() const;
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

        void useDefinition() override;

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
        void removeShaderInstance();
        void removeLightInstance();
        void removeSpriteInstance();
        void removeScriptInstance();
        void removePhysicsObjectInstance();
        void removeFontInstance();

        void replaceAssetUuid(string uuid);
    private:

        AudioInstance* mAudioInstance;
        PathInstance* mPathInstance;
        AssimpModelInstance* mModelInstance;
        ShaderInstance* mShaderInstance;
        LightInstance* mLightInstance;
        SpriteInstance* mSpriteInstance;
        ScriptInstance* mScriptInstance;
        PhysicsObjectInstance* mPhysicsObjectInstance;
        FontInstance* mFontInstance;
        Transform3D* mTransform;

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
        void loadChildrenFromDefinition(SceneObjectDefinition* definition);
        bool mFollowsCamera;

        void initialTransform();
    };
}
