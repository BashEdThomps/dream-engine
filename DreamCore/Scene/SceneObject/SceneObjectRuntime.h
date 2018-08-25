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
#include "../../Common/ILoggable.h"

#include "../../Components/Transform3D.h"

using std::vector;
using std::unique_ptr;
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
    class LuaScriptInstance;
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

    class SceneObjectRuntime : public Runtime, ILoggable
    {

    private:
        unique_ptr<AudioInstance> mAudioInstance;
        unique_ptr<PathInstance> mPathInstance;
        unique_ptr<AssimpModelInstance> mModelInstance;
        unique_ptr<ShaderInstance> mShaderInstance;
        unique_ptr<LightInstance> mLightInstance;
        unique_ptr<SpriteInstance> mSpriteInstance;
        unique_ptr<LuaScriptInstance> mScriptInstance;
        unique_ptr<PhysicsObjectInstance> mPhysicsObjectInstance;
        unique_ptr<FontInstance> mFontInstance;
        Transform3D mTransform;
        Transform3D mOffsetTransform;

        vector<Event> mEventQueue;
        vector<string> mAssetDefinitionUuidLoadQueue;
        vector<unique_ptr<SceneObjectRuntime>> mChildRuntimes;

        SceneRuntime* mSceneRuntimeHandle;
        SceneObjectRuntime* mParentRuntimeHandle;
        string mProjectPath;

        bool mLoaded;
        bool mHasFocus;

        void setAssetDefinitionLoadQueue(vector<string> loadQueue);
        void loadChildrenFromDefinition(SceneObjectDefinition *definitionHandle);
        bool mFollowsCamera;

    public:
        SceneObjectRuntime(SceneObjectDefinition* sdHandle, SceneRuntime* sceneRuntimeHandle = nullptr);

        ~SceneObjectRuntime() override;

        void collectGarbage() override;

        SceneRuntime* getSceneRuntimeHandle();
        SceneObjectDefinition* getSceneObjectDefinitionHandle();

        void createAssetInstances();
        void createAssetInstanceFromAssetDefinitionByUuid(string);
        void createAssetInstance(IAssetDefinition*);
        void createPathInstance(PathDefinition*);
        void createAudioInstance(AudioDefinition*);
        void createModelInstance(ModelDefinition*);
        void createScriptInstance(ScriptDefinition*);
        void createShaderInstance(ShaderDefinition*);
        void createPhysicsObjectInstance(PhysicsObjectDefinition*);
        void createLightInstance(LightDefinition*);
        void createFontInstance(FontDefinition*);
        void createSpriteInstance(SpriteDefinition*);

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

        //void setPathInstance(PathInstance*);
        //void setAudioInstance(AudioInstance*);
        //void setModelInstance(AssimpModelInstance*);
        //void setScriptInstance(LuaScriptInstance*);
        //void setShaderInstance(ShaderInstance*);
        //void setPhysicsObjectInstance(PhysicsObjectInstance*);
        //void setLightInstance(LightInstance*);
        //void setSpriteInstance(SpriteInstance*);
        //void setFontInstance(FontInstance*);

        PathInstance* getPathInstance();
        AudioInstance* getAudioInstance();
        AssimpModelInstance* getModelInstance();
        LuaScriptInstance* getScriptInstance();
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

        string getTransformType();
        void setTransformType(string);

        Transform3D& getTransform();
        void setTransform(Transform3D);

        bool hasFocus();
        void setHasFocus(bool);

        bool getLoadedFlag();
        void setLoadedFlag(bool);

        bool hasEvents();
        void sendEvent(Event);
        vector<Event> getEventQueue();
        void clearEventQueue();

        SceneObjectRuntime* getChildRuntimeHandleByUuid(string);

        int countAllChildren();
        size_t countChildren();
        void addChildRuntimeHandle(SceneObjectRuntime*);
        void removeChildRuntimeHandle(SceneObjectRuntime*);
        vector<SceneObjectRuntime*> getChildRuntimeHandles();
        bool isChildOf(SceneObjectRuntime*);

        bool isParentOf(SceneObjectRuntime* child);
        void setParentRuntimeHandle(SceneObjectRuntime* parent);
        SceneObjectRuntime* getParentRuntimeHandle();

        void* applyToAll(function<void*(SceneObjectRuntime*)>);
        bool applyToAll(function<bool(SceneObjectRuntime*)>);

        void useDefinition(IDefinition* iDefinitionHandle) override;

        bool followsCamera() const;
        void setFollowsCamera(bool followsCamera);
        void walk(float,float);
    };
}
