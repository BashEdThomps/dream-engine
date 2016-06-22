#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <vector>
#include "../ExternalLibs/JSON/json.hpp"
#include "../Asset/AssetInstance.h"

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
	
namespace Asset {
	class AssetInstance;
}
	
namespace Scene {
	
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
		Asset::AssetInstance* mAudioAssetInstance;
		Asset::AssetInstance* mAnimationAssetInstance;
		Asset::AssetInstance* mModelAssetInstance;
		Asset::AssetInstance* mScriptAssetInstance;
		Asset::AssetInstance* mShaderAssetInstance;
		Asset::AssetInstance* mPhysicsObjectAssetInstance;
		Asset::AssetInstance* mLightAssetInstance;
		
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
		
		void setAnimationAssetInstance(Asset::AssetInstance*);
		Asset::AssetInstance* getAnimationAssetInstance();
		
		void setAudioAssetInstance(Asset::AssetInstance*);
		Asset::AssetInstance* getAudioAssetInstance();
		
		void setModelAssetInstance(Asset::AssetInstance*);
		Asset::AssetInstance* getModelAssetInstance();
		bool hasModelAssetInstance();
		
		void setScriptAssetInstance(Asset::AssetInstance*);
		Asset::AssetInstance* getScriptAssetInstance();
		bool hasScriptAssetInstance();
		
		void setShaderAssetInstance(Asset::AssetInstance*);
		Asset::AssetInstance* getShaderAssetInstance();
		bool hasShaderAssetInstance();
		
		void setPhysicsObjectAssetInstance(Asset::AssetInstance*);
		Asset::AssetInstance* getPhysicsObjectAssetInstance();
		
		void setLightAssetInstance(Asset::AssetInstance*);
		Asset::AssetInstance* getLightAssetInstance();
		
		void setAssetInstanceParentToThis(Asset::AssetInstance*);
		
		std::vector<std::string> getAssetInstanceUUIDsToLoad();
		
		std::string getTransformType();
		void setTransformType(std::string);
		
	}; // End of SceneObject
	
} // End of Scene
} // End of Dream

#endif // NODE_H
