/*
* node.h
*/
#ifndef NODE_H
#define NODE_H

#include <iostream>
#include "../JSON/json.hpp"

#define NODE_X 0
#define NODE_Y 1
#define NODE_Z 2

namespace Dream {
	namespace Scene {
		class SceneObject {
		protected:
			SceneObject* mParent;
			std::string  mName;
			std::string  mPath;
			float        mTranslation[3];
			float        mRotation[3];
		public:
			SceneObject  ();
			SceneObject  (nlohmann::json);
			~SceneObject ();

			void initTranslationRotation();

			int  hasValidPath();
			std::string getPath();
			void setPath(std::string);
			void generatePath();

			int  hasValidName();
			bool hasName(std::string);
			std::string getName();
			void setName(std::string);

			void print();

			void setParent(SceneObject*);
			SceneObject* getParent();

			void setTranslation(float, float, float);
			void setRotation(float, float, float);
			void setPolarTranslation(float, float);

			bool isChildOf(SceneObject*);
			bool isParentOf(SceneObject*);
			int  countChildren();
		};
	}
}

#endif // NODE_H
