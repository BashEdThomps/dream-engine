#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <vector>
#include "../JSON/json.hpp"

#define NODE_X 0
#define NODE_Y 1
#define NODE_Z 2

#define SCENE_OBJECT_JSON_UUID        "uuid"
#define SCENE_OBJECT_JSON_NAME        "name"
#define SCENE_OBJECT_JSON_CHILDREN    "children"
#define PATH_DELIMETER                "::"

namespace Dream {
	namespace Scene {
		class SceneObject {
		protected:

			SceneObject* mParent;
			std::vector<SceneObject*> mChildren;

			std::string  mUUID;
			std::string  mName;
			std::string  mPath;
			float        mTranslation[3];
			float        mRotation[3];

		public:

			SceneObject  ();
			SceneObject  (nlohmann::json);
			~SceneObject ();

			void init();
			void initTranslationRotation();

			bool hasUUID(std::string);
			void setUUID(std::string);
			std::string getUUID();

			bool hasName(std::string);
			void setName(std::string);
			std::string getName();

			void showStatus();

			std::string getNameUUIDString();

			void setTranslation(float, float, float);
			float* getTranslation();

			void setRotation(float, float, float);
			float* getRotation();

			int  countAllChildren();
			int  countChildren();
			void addChild(SceneObject*);
			void removeChild(SceneObject*);
			bool isChildOf(SceneObject*);
			bool isChildOfDeep(SceneObject*);
			void getChildrenVectorDeep(std::vector<SceneObject*>*);

			bool isParentOf(SceneObject*);
			bool isParentOfDeep(SceneObject*);
			void setParent(SceneObject*);
			SceneObject* getParent();
			void generatePath();
			std::string getPath();
		};
	}
}

#endif // NODE_H
