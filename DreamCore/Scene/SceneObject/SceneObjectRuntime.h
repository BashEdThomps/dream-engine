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
        shared_ptr<AudioInstance> mAudioInstance;
        shared_ptr<PathInstance> mPathInstance;
        shared_ptr<AssimpModelInstance> mModelInstance;
        shared_ptr<ShaderInstance> mShaderInstance;
        shared_ptr<LightInstance> mLightInstance;
        shared_ptr<SpriteInstance> mSpriteInstance;
        shared_ptr<LuaScriptInstance> mScriptInstance;
        shared_ptr<PhysicsObjectInstance> mPhysicsObjectInstance;
        shared_ptr<FontInstance> mFontInstance;
        Transform3D mTransform;
        Transform3D mOffsetTransform;

        vector<Event> mEventQueue;
        vector<string> mAssetDefinitionUuidLoadQueue;
        vector<shared_ptr<SceneObjectRuntime>> mChildRuntimes;

        shared_ptr<SceneRuntime> mSceneRuntime;
        shared_ptr<SceneObjectRuntime> mParentRuntime;
        shared_ptr<SceneObjectRuntime> mThisShared;
        string mProjectPath;

        bool mLoaded;
        bool mHasFocus;

        void setAssetDefinitionLoadQueue(vector<string> loadQueue);
        void loadChildrenFromDefinition(shared_ptr<SceneObjectDefinition> definition);
        bool mFollowsCamera;

    public:
        SceneObjectRuntime(shared_ptr<SceneObjectDefinition> sd, shared_ptr<SceneRuntime> sceneRuntime = nullptr);

        ~SceneObjectRuntime() override;

        void collectGarbage() override;

        shared_ptr<SceneRuntime> getSceneRuntime();
        shared_ptr<SceneObjectDefinition> getSceneObjectDefinition();

        void createAssetInstances();
        void createAssetInstanceFromAssetDefinitionByUuid(string);
        void createAssetInstance(shared_ptr<IAssetDefinition>);
        void createPathInstance(shared_ptr<PathDefinition>);
        void createAudioInstance(shared_ptr<AudioDefinition>);
        void createModelInstance(shared_ptr<ModelDefinition>);
        void createScriptInstance(shared_ptr<ScriptDefinition>);
        void createShaderInstance(shared_ptr<ShaderDefinition>);
        void createPhysicsObjectInstance(shared_ptr<PhysicsObjectDefinition>);
        void createLightInstance(shared_ptr<LightDefinition>);
        void createFontInstance(shared_ptr<FontDefinition>);
        void createSpriteInstance(shared_ptr<SpriteDefinition>);

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

        shared_ptr<PathInstance> getPathInstance();
        shared_ptr<AudioInstance> getAudioInstance();
        shared_ptr<AssimpModelInstance> getModelInstance();
        shared_ptr<LuaScriptInstance> getScriptInstance();
        shared_ptr<ShaderInstance> getShaderInstance();
        shared_ptr<PhysicsObjectInstance> getPhysicsObjectInstance();
        shared_ptr<LightInstance> getLightInstance();
        shared_ptr<SpriteInstance> getSpriteInstance();
        shared_ptr<FontInstance> getFontInstance();

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

        shared_ptr<SceneObjectRuntime> getChildRuntimeByUuid(string);

        int countAllChildren();
        size_t countChildren();
        void addChildRuntime(shared_ptr<SceneObjectRuntime>);
        void removeChildRuntime(shared_ptr<SceneObjectRuntime>);
        vector<shared_ptr<SceneObjectRuntime>> getChildRuntimes();
        bool isChildOf(shared_ptr<SceneObjectRuntime>);

        bool isParentOf(shared_ptr<SceneObjectRuntime> child);
        void setParentRuntime(shared_ptr<SceneObjectRuntime> parent);
        shared_ptr<SceneObjectRuntime> getParentRuntime();

        shared_ptr<SceneObjectRuntime> applyToAll
        (function<shared_ptr<SceneObjectRuntime>(shared_ptr<SceneObjectRuntime>)>);

        bool applyToAll
        (function<bool(shared_ptr<SceneObjectRuntime>)>);

        void useDefinition(shared_ptr<IDefinition> iDefinition) override;

        bool followsCamera() const;
        void setFollowsCamera(bool followsCamera);
        void walk(float,float);
    };
}
