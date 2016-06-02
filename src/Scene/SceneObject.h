#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <vector>
#include "../JSON/json.hpp"
#include "../Asset/AssetInstance.h"

#define NODE_X 0
#define NODE_Y 1
#define NODE_Z 2

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

#define SCENE_OBJECT_POSITION_TYPE          "position_type"
#define SCENE_OBJECT_POSITION_TYPE_OFFSET   "offset"
#define SCENE_OBJECT_POSITION_TYPE_ABSOLUTE "absolute"

#define SCENE_OBJECT_ASSET_INSTANCES "assetInstances"

namespace Dream {
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
		std::string  mPositionType;
		
		// Position
		float mTranslation[3];
		float mRotation[3];
		float mScale[3];
		
		// Asset Instances
		Asset::AssetInstance* mAnimationAssetInstance;
		Asset::AssetInstance* mAudioAssetInstance;
		Asset::AssetInstance* mModelAssetInstance;
		Asset::AssetInstance* mScriptAssetInstance;
		Asset::AssetInstance* mShaderAssetInstance;
		Asset::AssetInstance* mCollisionShapeAssetInstance;
		
	public:
		SceneObject  ();
		SceneObject  (nlohmann::json);
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
		void         setTranslation(float, float, float);
		float*       getTranslation();
		void         setRotation(float, float, float);
		float*       getRotation();
		void         setScale(float, float, float);
		float*       getScale();
		void         resetTranslationRotation();
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
		
		void setScriptAssetInstance(Asset::AssetInstance*);
		Asset::AssetInstance* getScriptAssetInstance();
		
		void setShaderAssetInstance(Asset::AssetInstance*);
		Asset::AssetInstance* getShaderAssetInstance();
		
		void setCollisionShapeAssetInstance(Asset::AssetInstance*);
		Asset::AssetInstance* getCollisionShapeAssetInstance();
		
		std::vector<std::string> getAssetInstanceUUIDsToLoad();
		bool hasModelAssetInstance();
		bool hasShaderAssetInstance();
		bool hasScriptAssetInstance();
	}; // End of SceneObject
	
} // End of Scene
} // End of Dream

#endif // NODE_H
