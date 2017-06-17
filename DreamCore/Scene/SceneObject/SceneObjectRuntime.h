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
using std::unique_ptr;
using glm::vec3;

namespace Dream
{
    class SceneObject;
    class Scene;
    class Event;
    class AudioInstance;
    class AnimationInstance;
    class AssimpModelInstance;
    class LightInstance;
    class ShaderInstance;
    class SpriteInstance;
    class FontInstance;
    class PhysicsObjectInstance;
    class LuaScriptInstance;
    class AssetDefinition;
    class SceneRuntime;

    class SceneObjectRuntime : public Runtime
    {

    private:
        unique_ptr<AudioInstance> mAudioInstance;
        unique_ptr<AnimationInstance> mAnimationInstance;
        unique_ptr<AssimpModelInstance> mModelInstance;
        unique_ptr<ShaderInstance> mShaderInstance;
        unique_ptr<LightInstance> mLightInstance;
        unique_ptr<SpriteInstance> mSpriteInstance;
        unique_ptr<LuaScriptInstance> mScriptInstance;
        unique_ptr<PhysicsObjectInstance> mPhysicsObjectInstance;
        unique_ptr<FontInstance> mFontInstance;
        unique_ptr<Transform3D> mTransform;

        vector<Event> mEventQueue;
        vector<string> mAssetDefinitionUuidsToLoad;

        SceneObject *mOwnerHandle;

        bool mLoaded;
        bool mHasFocus;
        bool mDelete;

    public:
        SceneObjectRuntime(SceneObject *ownerObjectHandle = nullptr);

        ~SceneObjectRuntime();
        void deleteChildRuntimes();
        void deleteAssetInstances();
        void cleanUp();
        void cleanUpEvents();
        void resetRuntime();

        SceneRuntime* getSceneRuntimeHandle();
        SceneObjectRuntime* getParentObjectRuntimeHandle();

        void createAssetInstances();
        void createAssetInstanceFromAssetDefinitionByUuid(string);
        void createAssetInstance(AssetDefinition*);
        void createAnimationInstance(AssetDefinition*);
        void createAudioInstance(AssetDefinition*);
        void createModelInstance(AssetDefinition*);
        void createScriptInstance(AssetDefinition*);
        void createShaderInstance(AssetDefinition*);
        void createPhysicsObjectInstance(AssetDefinition*);
        void createLightInstance(AssetDefinition*);
        void createFontInstance(AssetDefinition*);
        void createSpriteInstance(AssetDefinition*);

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

        void setAnimationInstance(AnimationInstance*);
        AnimationInstance* getAnimationInstance();
        bool hasAnimationInstance();

        void setAudioInstance(AudioInstance*);
        AudioInstance* getAudioInstance();
        bool hasAudioInstance();

        void setModelInstance(AssimpModelInstance*);
        AssimpModelInstance* getModelInstance();
        bool hasModelInstance();

        void setScriptInstance(LuaScriptInstance*);
        LuaScriptInstance* getScriptInstance();
        bool hasScriptInstance();

        void setShaderInstance(ShaderInstance*);
        ShaderInstance* getShaderInstance();
        bool hasShaderInstance();

        void setPhysicsObjectInstance(PhysicsObjectInstance*);
        PhysicsObjectInstance* getPhysicsObjectInstance();
        bool hasPhysicsObjectInstance();

        void setLightInstance(LightInstance*);
        LightInstance* getLightInstance();
        bool hasLightInstance();

        void setSpriteInstance(SpriteInstance*);
        SpriteInstance* getSpriteInstance();
        bool hasSpriteInstance();

        void setFontInstance(FontInstance*);
        FontInstance* getFontInstance();
        bool hasFontInstance();

        void addAssetDefinitionUuidToLoad(string);
        vector<string> getAssetDefinitionUuidsToLoad();

        string getTransformType();
        void setTransformType(string);

        Transform3D* getTransform();
        void setTransform(Transform3D*);

        bool hasFocus();
        void setHasFocus(bool);

        void setDeleteFlag(bool);
        bool getDeleteFlag();

        bool getLoadedFlag();
        void setLoadedFlag(bool);

        bool hasEvents();
        void sendEvent(Event);
        vector<Event>& getEventQueue();


        void loadAssetInstances();

    };
}
