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
		float*      mVertexBuffers[VERT_BUFFERS];
		Camera*     mCamera;
		int         mRootNodeIndex;
	public:
		// Init
		Scenegraph(std::string);
		~Scenegraph();

		// VertexBuffer
		int addVertexBuffer(void*);
		int getNextAvailableVertexBuffer();
		int getIndexOfVertexBuffer(void*);
		int removeVertexBuffer(void*);

		// Display/Debug
		void printGraph ();

		// Node Management
		void  generatePathForNode(Node*);
		void  setRootNodeIndex(int);
		void  setRootNode(Node*);
		int   getNextAvailableNode();
		int   getIndexOfNode(Node*);
		Node* getRootNode();
		int   getRootNodeIndex();
		int   isRootNode(Node*);
		Node* createNode();
		Node* createNodeWithName(std::string);
		void  removeNode(Node*);
		int   countChildrenOfNode(Node*);
		Node* getNodeByName(std::string);

		// Updating
		void updatePaths();
		void traverseNodeVector(void (*)(Node*, void*), void*);
		int  traversePath(std::string, void (*)(Node*,  void*), void*);

		// Node Relationships
		int  isNodeChildOf(Node*,Node*);
		int  isNodeParentOf(Node*,Node*);
		void setParentNode(Node*,Node*);
	};
}
#endif // GRAPH_H
