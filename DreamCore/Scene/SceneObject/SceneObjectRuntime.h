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

    private:
        shared_ptr<AudioInstance> mAudioInstance;
        shared_ptr<PathInstance> mPathInstance;
        shared_ptr<AssimpModelInstance> mModelInstance;
        shared_ptr<ShaderInstance> mShaderInstance;
        shared_ptr<LightInstance> mLightInstance;
        shared_ptr<SpriteInstance> mSpriteInstance;
        shared_ptr<ScriptInstance> mScriptInstance;
        shared_ptr<PhysicsObjectInstance> mPhysicsObjectInstance;
        shared_ptr<FontInstance> mFontInstance;
        shared_ptr<Transform3D> mTransform;

        vector<Event> mEventQueue;
        vector<string> mAssetDefinitionUuidLoadQueue;
        vector<shared_ptr<SceneObjectRuntime>> mChildRuntimes;

        shared_ptr<SceneRuntime> mSceneRuntime;
        shared_ptr<SceneObjectRuntime> mParentRuntime;
        string mProjectPath;

        bool mLoaded;
        bool mHasFocus;

        void setAssetDefinitionLoadQueue(vector<string> loadQueue);
        void loadChildrenFromDefinition(const shared_ptr<SceneObjectDefinition>& definition);
        bool mFollowsCamera;

        void initialTransform();
    public:
        SceneObjectRuntime(const shared_ptr<SceneObjectDefinition>& sd, const shared_ptr<SceneRuntime>& sceneRuntime = nullptr);
        ~SceneObjectRuntime() override;

        void collectGarbage() override;

        const shared_ptr<SceneRuntime>& getSceneRuntime();
        shared_ptr<SceneObjectDefinition> getSceneObjectDefinition();

        void createAssetInstances();
        void createAssetInstanceFromAssetDefinitionByUuid(string);
        void createAssetInstance(const shared_ptr<IAssetDefinition>&);
        void createPathInstance(const shared_ptr<PathDefinition>&);
        void createAudioInstance(const shared_ptr<AudioDefinition>&);
        void createModelInstance(const shared_ptr<ModelDefinition>&);
        void createScriptInstance(const shared_ptr<ScriptDefinition>&);
        void createShaderInstance(const shared_ptr<ShaderDefinition>&);
        void createPhysicsObjectInstance(const shared_ptr<PhysicsObjectDefinition>&);
        void createLightInstance(const shared_ptr<LightDefinition>&);
        void createFontInstance(const shared_ptr<FontDefinition>&);
        void createSpriteInstance(const shared_ptr<SpriteDefinition>&);

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

        const shared_ptr<PathInstance>& getPathInstance();
        const shared_ptr<AudioInstance>& getAudioInstance();
        const shared_ptr<AssimpModelInstance>& getModelInstance();
        const shared_ptr<ScriptInstance>& getScriptInstance();
        const shared_ptr<ShaderInstance>& getShaderInstance();
        const shared_ptr<PhysicsObjectInstance>& getPhysicsObjectInstance();
        const shared_ptr<LightInstance>& getLightInstance();
        const shared_ptr<SpriteInstance>& getSpriteInstance();
        const shared_ptr<FontInstance>& getFontInstance();

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

        shared_ptr<Transform3D> getTransform();
        void setTransform(shared_ptr<Transform3D>);

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
        void addChildRuntime(const shared_ptr<SceneObjectRuntime>&);
        void removeChildRuntime(const shared_ptr<SceneObjectRuntime>&);
        vector<shared_ptr<SceneObjectRuntime>> getChildRuntimes();
        bool isChildOf(const shared_ptr<SceneObjectRuntime>&);

        bool isParentOf(const shared_ptr<SceneObjectRuntime>& child);
        void setParentRuntime(const shared_ptr<SceneObjectRuntime>& parent);
        const shared_ptr<SceneObjectRuntime>& getParentRuntime();

        shared_ptr<SceneObjectRuntime> applyToAll
        (function<shared_ptr<SceneObjectRuntime>(shared_ptr<SceneObjectRuntime> )>);

        bool applyToAll
        (function<bool(shared_ptr<SceneObjectRuntime>)>);

        void useDefinition(shared_ptr<IDefinition> iDefinition) override;

        bool followsCamera() const;
        void setFollowsCamera(bool followsCamera);
        void walk(float,float);
        void drive(float,float);
    };
}
