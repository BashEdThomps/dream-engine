#ifndef NODE_H
#define NODE_H

#include <iostream>
#include "../JSON/json.hpp"

#define NODE_X 0
#define NODE_Y 1
#define NODE_Z 2

#define SCENE_OBJECT_JSON_UUID        "uuid"
#define SCENE_OBJECT_JSON_PARENT_UUID "parentUuid"
#define SCENE_OBJECT_JSON_NAME        "name"
#define SCENE_OBJECT_JSON_CHILDREN    "children"
#define PATH_DELIMETER                "/"

namespace Dream {
	namespace Scene {
		class SceneObject {
		protected:

			std::string  mParentUUID;
			std::string  mUUID;
			std::string  mName;
			float        mTranslation[3];
			float        mRotation[3];

		public:

			SceneObject  ();
			SceneObject  (nlohmann::json);
			~SceneObject ();

			void init();
			void initTranslationRotation();


			void setParentUUID(std::string);
			std::string getParentUUID();

			bool hasUUID(std::string);
			void setUUID(std::string);
			std::string getUUID();

			bool hasValidName();
			bool hasName(std::string);
			std::string getName();
			void setName(std::string);

			void showStatus();

			void setTranslation(float, float, float);
			float* getTranslation();

			void setRotation(float, float, float);
			float* getRotation();
		};
	}
}

#endif // NODE_H
