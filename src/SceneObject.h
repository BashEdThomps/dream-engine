#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <vector>

#include "Transform3D.h"

#include "Components/Animation/AnimationInstance.h"
#include "Components/Audio/AudioInstance.h"
#include "Components/Graphics/AssimpModelInstance.h"
#include "Components/Graphics/LightInstance.h"
#include "Components/Graphics/ShaderInstance.h"
#include "Components/Graphics/SpriteInstance.h"
#include "Components/Graphics/FontInstance.h"
#include "Components/Physics/PhysicsObjectInstance.h"
#include "Components/Scripting/LuaScriptInstance.h"

namespace Dream {

  using namespace std;

  class SceneObject {
  protected:
      SceneObject* mParent;
      vector<SceneObject*> mChildren;
      vector<string> mAssetInstanceUUIDsToLoad;
      bool mHasFocus;
      // Metadata
      string mUUID;
      string mName;
      string mPath;
      Transform3D *mTransform;
      // Asset Instances
      AudioInstance *mAudioInstance;
      AnimationInstance *mAnimationInstance;
      AssimpModelInstance *mModelInstance;
      ShaderInstance *mShaderInstance;
      LightInstance *mLightInstance;
      SpriteInstance *mSpriteInstance;
      LuaScriptInstance *mScriptInstance;
      PhysicsObjectInstance *mPhysicsObjectInstance;
      FontInstance *mFontInstance;
  public:
      SceneObject(nlohmann::json);
      SceneObject();
      ~SceneObject();

      bool init();
      void loadAssetInstances(nlohmann::json);
      bool initAssetInstances();

      bool hasUUID(string);
      void setUUID(string);
      string getUUID();

      bool hasName(string);
      void setName(string);
      string getName();

      string getNameUUIDString();

      void showStatus();

      vector<float> getTranslation();
      void setTranslation(float, float, float);
      void setTranslation(vector<float>);
      void resetTranslation();

      vector<float> getRotation();
      void setRotation(float, float, float);
      void setRotation(vector<float>);
      void resetRotation();

      vector<float> getScale();
      void setScale(float, float, float);
      void setScale(vector<float>);
      void resetScale();

      void resetTransform();

      int countAllChildren();
      size_t countChildren();
      void addChild(SceneObject*);
      void removeChild(SceneObject*);
      bool isChildOf(SceneObject*);
      bool isChildOfDeep(SceneObject*);
      void getChildrenVectorDeep(vector<SceneObject*>*);
      bool isParentOf(SceneObject*);
      bool isParentOfDeep(SceneObject*);
      void setParent(SceneObject*);
      SceneObject* getParent();

      void generatePath();
      string  getPath();

      void setAnimationInstance(AnimationInstance*);
      AnimationInstance* getAnimationInstance();

      void setAudioInstance(AudioInstance*);
      AudioInstance* getAudioInstance();

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

      void setLightInstance(LightInstance*);
      LightInstance* getLightInstance();

      void setSpriteInstance(SpriteInstance*);
      SpriteInstance* getSpriteInstance();
      bool hasSpriteInstance();

      void setFontInstance(FontInstance*);
      FontInstance* getFontInstance();
      bool hasFontInstance();

      vector<string> getAssetInstanceUUIDsToLoad();

      string getTransformType();
      void setTransformType(string);

      Transform3D* getTransform();
      void setTransform(Transform3D*);

      bool hasFocus();
      void setHasFocus(bool);

  }; // End of SceneObject

} // End of Dream

#endif // NODE_H
