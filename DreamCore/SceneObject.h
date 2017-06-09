#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <vector>
#include <functional>
#include <memory>

#include "Transform3D.h"
#include "Constants.h"
#include "Event.h"
#include "Components/Animation/AnimationInstance.h"
#include "Components/Audio/AudioInstance.h"
#include "Components/Graphics/AssimpModelInstance.h"
#include "Components/Graphics/LightInstance.h"
#include "Components/Graphics/ShaderInstance.h"
#include "Components/Graphics/SpriteInstance.h"
#include "Components/Graphics/FontInstance.h"
#include "Components/Physics/PhysicsObjectInstance.h"
#include "Lua/LuaScriptInstance.h"

using namespace std;
namespace Dream
{
    class Scene;

    class SceneObject
    {
    protected:
        Scene* mScene;
        nlohmann::json mJson;
        vector<Event*> mEventQueue;
        bool mLoaded;
        SceneObject* mParentHandle;
        vector<SceneObject*> mChildren;
        vector<string> mAssetDefUuidsToLoad;
        bool mHasFocus;
        // Metadata
        string mUuid;
        string mName;
        unique_ptr<Transform3D> mTransform;
        bool mDelete;
        // Asset Instances
        unique_ptr<AudioInstance> mAudioInstance;
        unique_ptr<AnimationInstance> mAnimationInstance;
        unique_ptr<AssimpModelInstance> mModelInstance;
        unique_ptr<ShaderInstance> mShaderInstance;
        unique_ptr<LightInstance> mLightInstance;
        unique_ptr<SpriteInstance> mSpriteInstance;
        unique_ptr<LuaScriptInstance> mScriptInstance;
        unique_ptr<PhysicsObjectInstance> mPhysicsObjectInstance;
        unique_ptr<FontInstance> mFontInstance;
    public:
        SceneObject(Scene* scene = nullptr);
        SceneObject(Scene*, nlohmann::json);
        ~SceneObject();
        void deleteChildren();
        void deleteAssetInstances();

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

        nlohmann::json toJson();

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

#endif // NODE_H
