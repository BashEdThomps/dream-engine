#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include "Node.h"
#include "Camera.h"

#define NODES        256
#define VERT_BUFFERS 256

namespace DreamScenegraph {
	// Global Variables
	class Scenegraph {
	protected:
		std::string mName;
		Node*       mNodes[NODES];
		Camera*     mCamera;
		Node*       mRootNode;
	public:
		// Init
		Scenegraph(std::string);
		~Scenegraph();

		// Display/Debug
		void printGraph ();

		// Node Management
		void  generatePathForNode(Node*);
		void  setRootNode(Node*);
		int   getNextAvailableNodeIndex();
		int   getIndexOfNode(Node*);
		Node* getRootNode();
		int   getRootNodeIndex();
		bool   isRootNode(Node*);
		Node* createNode();
		Node* createNodeWithName(std::string);
		void  removeNode(Node*);
		int   countChildrenOfNode(Node*);
		Node* getNodeByName(std::string);

		// Updating
		void updatePaths();
		//void traverseNodeVector(void (*)(Node*, void*), void*);
		//int  traversePath(std::string, void (*)(Node*,  void*), void*);

		// Node Relationships

	};
}
#endif // GRAPH_H
