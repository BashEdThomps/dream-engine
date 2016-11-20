#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <vector>
#include "json.hpp"
#include "AssetInstance.h"

#define SO_X 0
#define SO_Y 1
#define SO_Z 2

#define SCENE_OBJECT_UUID     "uuid"
#define SCENE_OBJECT_NAME     "name"
#define SCENE_OBJECT_CHILDREN "children"
#define PATH_DELIMETER        "::"

#define SCENE_OBJECT_TRANSLATION "translation"
#define SCENE_OBJECT_ROTATION    "rotation"
#define SCENE_OBJECT_SCALE       "scale"

#define SCENE_OBJECT_X "x"
#define SCENE_OBJECT_Y "y"
#define SCENE_OBJECT_Z "z"

#define SCENE_OBJECT_TRANSFORM_TYPE          "transformType"
#define SCENE_OBJECT_TRANSFORM_TYPE_OFFSET   "offset"
#define SCENE_OBJECT_TRANSFORM_TYPE_ABSOLUTE "absolute"

#define SCENE_OBJECT_ASSET_INSTANCES "assetInstances"

namespace Dream {

  class SceneObject {
  protected:
    SceneObject* mParent;
    std::vector<SceneObject*> mChildren;
    std::vector<std::string>  mAssetInstanceUUIDsToLoad;

    // Metadata
    std::string  mUUID;
    std::string  mName;
    std::string  mPath;
    std::string  mTransformType;

    // Position
    std::vector<float> mTranslation;
    std::vector<float> mRotation;
    std::vector<float> mScale;

    // Asset Instances
    AssetInstance* mAudioAssetInstance;
    AssetInstance* mAnimationAssetInstance;
    AssetInstance* mModelAssetInstance;
    AssetInstance* mScriptAssetInstance;
    AssetInstance* mShaderAssetInstance;
    AssetInstance* mPhysicsObjectAssetInstance;
    AssetInstance* mLightAssetInstance;

  public:
    SceneObject  (nlohmann::json);
    SceneObject  ();
    ~SceneObject ();

    bool         init();
    void         loadAssetInstances(nlohmann::json);
    bool         initAssetInstances();

    bool         hasUUID(std::string);
    void         setUUID(std::string);
    std::string  getUUID();

    bool         hasName(std::string);
    void         setName(std::string);
    std::string  getName();

    std::string  getNameUUIDString();

    void         showStatus();

    std::vector<float> getTranslation();
    void         setTranslation(float, float, float);
    void         setTranslation(std::vector<float>);
    void         resetTranslation();

    std::vector<float> getRotation();
    void         setRotation(float, float, float);
    void         setRotation(std::vector<float>);
    void         resetRotation();

    std::vector<float> getScale();
    void         setScale(float, float, float);
    void         setScale(std::vector<float>);
    void         resetScale();

    void         resetTransform();

    int          countAllChildren();
    int          countChildren();
    void         addChild(SceneObject*);
    void         removeChild(SceneObject*);
    bool         isChildOf(SceneObject*);
    bool         isChildOfDeep(SceneObject*);
    void         getChildrenVectorDeep(std::vector<SceneObject*>*);
    bool         isParentOf(SceneObject*);
    bool         isParentOfDeep(SceneObject*);
    void         setParent(SceneObject*);
    SceneObject* getParent();

    void         generatePath();

    std::string  getPath();

    void setAnimationAssetInstance(AssetInstance*);
    AssetInstance* getAnimationAssetInstance();

    void setAudioAssetInstance(AssetInstance*);
    AssetInstance* getAudioAssetInstance();

    void setModelAssetInstance(AssetInstance*);
    AssetInstance* getModelAssetInstance();
    bool hasModelAssetInstance();

    void setScriptAssetInstance(AssetInstance*);
    AssetInstance* getScriptAssetInstance();
    bool hasScriptAssetInstance();

    void setShaderAssetInstance(AssetInstance*);
    AssetInstance* getShaderAssetInstance();
    bool hasShaderAssetInstance();

    void setPhysicsObjectAssetInstance(AssetInstance*);
    AssetInstance* getPhysicsObjectAssetInstance();

    void setLightAssetInstance(AssetInstance*);
    AssetInstance* getLightAssetInstance();

    void setAssetInstanceParentToThis(AssetInstance*);

    std::vector<std::string> getAssetInstanceUUIDsToLoad();

    std::string getTransformType();
    void setTransformType(std::string);

  }; // End of SceneObject

} // End of Dream

#endif // NODE_H
