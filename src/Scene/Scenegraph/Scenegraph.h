#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include "SceneObject.h"
#include "Camera.h"

#define NODES        256
#define VERT_BUFFERS 256

namespace Dream {
	namespace Scene {
		namespace Scenegraph {
			// Global Variables
			class Scenegraph {
			protected:
				std::string mName;
				SceneObject*       mSceneObjects[NODES];
				Camera*     mCamera;
				SceneObject*       mRootSceneObject;
			public:
				// Init
				Scenegraph(std::string);
				~Scenegraph();

				// Display/Debug
				void printGraph ();

				// SceneObject Management
				void  generatePathForSceneObject(SceneObject*);
				void  setRootSceneObject(SceneObject*);
				int   getNextAvailableSceneObjectIndex();
				int   getIndexOfSceneObject(SceneObject*);
				SceneObject* getRootSceneObject();
				int   getRootSceneObjectIndex();
				bool   isRootSceneObject(SceneObject*);
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
