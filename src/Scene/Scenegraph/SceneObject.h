/*
* node.h
*/
#ifndef NODE_H
#define NODE_H

#include <iostream>
//#include "../Resource.h"

#define NODE_X 0
#define NODE_Y 1
#define NODE_Z 2

namespace Dream {
	namespace Scene {
		namespace Scenegraph {
			class SceneObject {
			protected:
				SceneObject* mParent;
				std::string  mName;
				std::string  mPath;
				float        mTranslation[3];
				float        mRotation[3];
			public:
				SceneObject  ();
				SceneObject  (std::string);
				~SceneObject ();

				void init();
				void initTranslationRotation();

				int  hasValidPath();
				std::string getPath();
				void setPath(std::string);

				int  hasValidName();
				bool hasName(std::string);
				std::string getName();

				void print();

				void setParent(SceneObject*);
				SceneObject* getParent();

				void setTranslation(float, float, float);
				void setRotation(float, float, float);
				void setPolarTranslation(float, float);

				bool  isChildOf(SceneObject*);
				bool  isParentOf(SceneObject*);
				int countChildren();
				//void addResource(Dream::Resource*);
			};
		}
	}
}

#endif // NODE_H
