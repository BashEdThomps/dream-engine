#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <memory>

#include "../Common/Transform3D.h"
#include "../Common/Constants.h"
#include "../Common/Event.h"
#include "../Components/Animation/AnimationInstance.h"
#include "../Components/Audio/AudioInstance.h"
#include "../Components/Graphics/Model/AssimpModelInstance.h"
#include "../Components/Graphics/Light/LightInstance.h"
#include "../Components/Graphics/Shader/ShaderInstance.h"
#include "../Components/Graphics/Sprite/SpriteInstance.h"
#include "../Components/Graphics/Font/FontInstance.h"
#include "../Components/Physics/PhysicsObjectInstance.h"
#include "../Lua/LuaScriptInstance.h"

using namespace std;
namespace Dream
{
    class Scene;

    class SceneObject
    {
    protected:

        // Metadata
        nlohmann::json mJson;

        // Runtime Members
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
        vector<SceneObject*> mChildren;
        vector<Event*> mEventQueue;
        SceneObject* mParentHandle;
        Scene* mSceneHandle;
        bool mLoaded;
        bool mHasFocus;
        bool mDelete;
    public:
        SceneObject(Scene* scene = nullptr);
        SceneObject(Scene*, nlohmann::json);
        ~SceneObject();

        void deleteChildren();
        void deleteAssetInstances();

        Scene* getSceneHandle();

        void constructorInit();

        bool init();
        void loadJsonData(nlohmann::json);
        void loadAssetDefinitionsToLoadJsonData(nlohmann::json);
        void loadAssetInstances();

        bool hasUuid(string);
        void setUuid(string);
        string getUuid();

        bool hasName(string);
        void setName(string);
        string getName();

        string getNameAndUuidString();

        void showStatus();

        glm::vec3 getTranslation();
        void setTranslation(float, float, float);
        void setTranslation(glm::vec3);
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

        SceneObject* getChildByUuid(string);
        int countAllChildren();
        size_t countChildren();
        void addChild(SceneObject*);
        void removeChild(SceneObject*);
        bool isChildOf(SceneObject*);
        vector<SceneObject*> getChildren();
        bool isParentOf(SceneObject*);
        void setParent(SceneObject*);
        SceneObject* getParent();

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

        void addAssetDefUuidToLoad(string);
        vector<string> getAssetDefUuidsToLoad();

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
        void sendEvent(Event*);
        vector<Event*>* getEventQueue();

        nlohmann::json getJson();

        void* applyToAll(function<void*(SceneObject*)>);
        bool applyToAll(function<bool(SceneObject*)>);

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


        void cleanUp();
        void cleanUpEvents();

    }; // End of SceneObject

} // End of Dream

