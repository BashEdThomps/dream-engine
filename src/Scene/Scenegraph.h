#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H


#include <vector>
#include <iostream>
#include "SceneObject.h"

namespace Dream {
	namespace Scene {
		class Scenegraph {
		protected:
			std::vector<SceneObject*> mSceneObjects;
			SceneObject*              mRootSceneObject;
		public:
			Scenegraph();
			~Scenegraph();
			void         printGraph ();
			std::string  getPathDelimeter();
			void         setRootSceneObject(SceneObject*);
			SceneObject* getRootSceneObject();
			bool         isRootSceneObject(SceneObject*);
			SceneObject* createSceneObject(SceneObject*);
			void         removeSceneObject(SceneObject*);
			int          countChildrenOfSceneObject(SceneObject*);
			SceneObject* getSceneObjectByName(std::string);
			void         updatePaths();
			void         addSceneObjectTo(SceneObject*, SceneObject*);
			int          getNumberOfSceneObjects();
			bool         hasSceneObect(SceneObject*);
		};
	}
}
#endif // GRAPH_H
