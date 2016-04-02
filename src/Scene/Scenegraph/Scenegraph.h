#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <vector>
#include <iostream>
#include "SceneObject.h"
#include "Camera.h"

namespace Dream {
	namespace Scene {
		namespace Scenegraph {
			// Global Variables
			class Scenegraph {
			protected:
				std::vector<SceneObject*> mSceneObjects;
				Camera*                   mCamera;
				SceneObject*              mRootSceneObject;
			public:
				// Init
				Scenegraph();
				~Scenegraph();
				// Display/Debug
				void printGraph ();
				// SceneObject Management
				void  generatePathForSceneObject(SceneObject*);

				void         setRootSceneObject(SceneObject*);
				SceneObject* getRootSceneObject();
				bool         isRootSceneObject(SceneObject*);

				SceneObject* createSceneObject();
				SceneObject* createSceneObjectWithName(std::string);

				void  removeSceneObject(SceneObject*);
				int   countChildrenOfSceneObject(SceneObject*);

				SceneObject* getSceneObjectByName(std::string);

				// Updating
				void updatePaths();
				//void traverseSceneObjectVector(void (*)(SceneObject*, void*), void*);
				//int  traversePath(std::string, void (*)(SceneObject*,  void*), void*);

				// SceneObject Relationships
			};
		}
	}
}
#endif // GRAPH_H
