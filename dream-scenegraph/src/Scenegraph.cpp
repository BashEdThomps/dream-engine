/*
* graph.c
*/
#include <string>

#include "Node.h"
#include "Scenegraph.h"

#define PATH_DELIMETER '/'

namespace DreamScenegraph {

	Scenegraph::Scenegraph(std::string name = NULL) {
		mName = name;

		int i;
		for (i=0;i<NODES;i++) {
			mNodes[i] = NULL;
		}
		mCamera = new Camera();
		mRootNodeIndex = -1;
	}

	Node* Scenegraph::createNode() {
		int available = Scenegraph::GetNextAvailableNode(graph);
		if (available < 0) {
			fprintf(stderr,
				   "Error: Unable to create new node - tree is full\n");
			return NULL;
		}
		Node* retval = new Node();
		mNodes[available] = retval;
		return retval;
	}

	int Scenegraph::getNextAvailableNode() {
		int i;
		for (i=0; i<NODES; i++) {
			if (mNodes[i] == NULL) {
				return i;
			}
		}
		return -1;
	}

	int Scenegraph::getIndexOfNode(Node *node) {
		int i;
		for (i=0; i<NODES; i++) {
			if (mNodes[i] == node) {
				return i;
			}
		}
		return -1;
	}

	Node* Scenegraph::getRootNode() {
		return mNodes[mRootNodeIndex];
	}

	void Scenegraph::setRootNode(Node* root) {
		mRootNodeIndex = getIndexOfNode(root);
	}

	void Scenegraph::setRootNodeIndex(int root) {
		mRootNodeIndex = root;
		return;
	}

	void Scenegraph::removeNode(Node* node) {
		if (node == NULL) {
			fprintf(stderr,"Cannot remove NULL node from graph.\n");
			return;
		}

		int i = getIndexOfNode(node);
		if (i < 0) {
			fprintf(stderr,"Cannot find node in graph to remove it.\n");
			return;
		}

		delete node;
		mNodes[i] = NULL;
		return;
	}

	void Scenegraph::traverseNodeVector(void (*function)(dsgNode*, void*), void* arg) {
		int i;
		dsgNode* next;
		for (i=0;i<NODES;i++) {
			next = graph->nodes[i];
			if (next == NULL) {
				continue;
			}
			(*function)((void*)next, arg == NULL ? graph : arg);
		}
		return;
	}

	dsgNode* Scenegraph::getNodeByName(std::string name) {
		int i;
		dsgNode* next;
		for (i=0;i<NODES;i++) {
			next = graph->nodes[i];
			if (next == NULL) {
				continue;
			}
			if (next->name == NULL) {
				fprintf(stderr,"dsgScenegraphGetNodeByName: Cannot test node with NULL name\n");
				continue;
			}
			if (strcmp(graph->nodes[i]->name,name) == 0) {
				return graph->nodes[i];
			}
		}
		return NULL;
	}

	intScenegraph::TraversePath(dsgScenegraph* graph,  std::string path, void (*function)(dsgNode*, void*), void* arg) {
		 std::string start    = NULL;
		 std::string end      = NULL;
		int done       = 0;
		int nameLen    = 0;
		dsgNode* target = NULL;
		//fprintf(stdout,"Traversing path: %s\n",path);
		start = path+1;
		if (start == NULL) {
			fprintf(stderr,"Path is null\n");
			return done;
		}
		while (!done) {
			nameLen = 0;
			end = strchr(start+1, Scenegraph::PathDelimeter);
			if (end == NULL) {
				//fprintf(stdout,"Found end of string\n");
				end = path+strlen(path);
				done = 1;
			}
			end += 1;
			nameLen = (end-start-1);
			if (nameLen <= 0) {
				fprintf(stderr,"Traverse nameLen <= 0\n");
				break;
			}
			 std::string tmp = ( std::string)malloc(sizeof(char)*nameLen+1);
			memcpy(tmp,start,nameLen);
			tmp[nameLen] = '\0';
			//fprintf(stdout,"Traversed path to: %s\n",tmp);
			target =Scenegraph::GetNodeByName(graph,tmp);
			if (target != NULL) {
				function(target,arg);
				start = end;
			} else {
				fprintf(stderr,"%s not found while traversing path\n",tmp);
				break;
			}
		}
		return done;
	}

	int Scenegraph::CountChildrenOfNode(dsgScenegraph* graph, dsgNode* node) {
		int retval = 0;
		int i;
		dsgNode* next;
		for (i = 0; i<NODES; i++) {
			next = graph->nodes[i];
			if (next == NULL) {
				continue;
			}

			if (dsgScenegraphNodeIsChildOf(graph, node, next)) {
				retval++;
			}
		}
		return retval;
	}

	voidScenegraph::UpdatePaths(dsgScenegraph* graph) {
		int i;
		dsgNode* next;
		for (i=0;i<NODES;i++) {
			next = graph->nodes[i];
			if (next != NULL) {
				dsgScenegraphGeneratePathForNode(graph, next);
			}
		}
		return;
	}

	void Scenegraph::GeneratePathForNode(dsgScenegraph* graph, dsgNode* node) {
		if (node == NULL) {
			fprintf(stderr,"Cannot generate path for NULL node.\n");
			return;
		}

		if (graph == NULL) {
			fprintf(stderr,"Cannot generate path for NULL graph.\n");
			return;
		}

		if (node->name == NULL) {
			fprintf(stderr,"Cannot generate path for node with no name.\n");
			return;
		}

		if (node->path != NULL) {
			free(node->path);
			node->path = NULL;
		}

		 std::string pathBuffer = ( std::string)malloc(sizeof(char)*STR_BUF_SIZE);
		// Node has no parent
		if (node->parentIndex == -1) {
			// and is not graph root
			if (!dsgScenegraphIsRootNode(graph,node)) {
				fprintf(stderr, "Cannot generate path for non-root node with no parent.\n");
				return;
			}
			fprintf(stdout,"Generating path for node root %s\n", node->name);
			memcpy(pathBuffer, &dsgScenegraphPathDelimeter, 1);
			pathBuffer[2] = '\0';
			strncat(pathBuffer, node->name, strlen(node->name));
		} else {
			dsgNode* parent = graph->nodes[node->parentIndex];
			if (parent == NULL) {
				fprintf(stderr,"Error finding node parent to create path\n");
				return;
			}
			fprintf(stdout,"Generating path for node %s with parent %s\n", node->name, parent->name);
			memcpy(pathBuffer, parent->path, strlen(parent->path)+1);
			strncat(pathBuffer, &dsgScenegraphPathDelimeter, strlen(&dsgScenegraphPathDelimeter));
			strncat(pathBuffer, node->name, strlen(node->name));
		}

		// Trim buffer fat
		int compressedLength =  strlen(pathBuffer)+1;
		 std::string pathBufferCompressed = ( std::string)malloc(sizeof(char)*compressedLength);
		memcpy(pathBufferCompressed,pathBuffer,compressedLength);
		pathBufferCompressed[compressedLength-1] = '\0';
		node->path = pathBufferCompressed;
		free(pathBuffer);

		fprintf(stdout,"Generated path: %s\n",node->path);
		return;
	}

	int Scenegraph::isRootNode(Node* node) {
		return mRootNodeIndex == getIndexOfNode(node);
	}

	int Scenegraph::addVertexBuffer(void* buffer) {
		fprintf(stdout,"Adding vertex buffer to graph\n");
		int index = Scenegraph::getNextAvailableVertexBuffer();
		if (index < 0) {
			fprintf(stderr,"Unable to add vertex buffer, graph is full\n");
			return -1;
		}
		mVertexBuffers[index] = (float*)buffer;
		return index;
	}

	int Scenegraph::getNextAvailableVertexBuffer() {
		int retval = -1;
		int i;
		for (i=0; i<VERT_BUFFERS; i++) {
			if (mVertexBuffers[i] == NULL) {
				retval = i;
				break;
			}
		}
		return retval;
	}

	int Scenegraph::getIndexOfVertexBuffer(void* buffer) {
		if(buffer == NULL) {
			return -1;
		}

		int retval = -1;
		int i;
		for (i=0; i<VERT_BUFFERS; i++) {
			if (mVertexBuffers[i] == buffer) {
				retval = i;
				break;
			}
		}
		return retval;
	}

	int Scenegraph::removeVertexBuffer(void* buffer) {
		if (buffer == NULL) {
			return -1;
		}

		int index = Scenegraph::getIndexOfNode(buffer);
		if (index < 0) {
			std::cerr << "Cannot remove vertex buffer from graph "
			          << mName != NULL ? graph->name : "Unamed Graph"
					  << ", not found"
					  << std::endl;
			return -1;
		}

		delete mVertexBuffers[index];
		mVertexBuffers[index] = NULL;
		return index;
	}

	int Scenegraph::nodeIsParentOf (Node* parent, Node* child) {
		int retval = 0;
		if (parent == NULL || !child->hasValidName() || !child->hasValidPath()) {
			return retval;
		}
		retval = child->getParentIndex() == getIndexOfNode(parent);
		return retval;
	}

	int Scenegraph::nodeIsChildOf(Node* parent, Node* child) {
		if (parent == NULL || child == NULL ||
		    child->hasValidPath() || child->hasValidName()) {
			return 0;
		}
		return child->getParentNodeIndex() == getIndexOfNode(parent);
	}

	void Scenegraph::setParentNode(Node* parent, Node* node) {
		dsgNodeSetParentIndex(dsgScenegraphGetIndexOfNode(graph,parent),node);
		return;
	}

	void Scenegraph::printGraph() {
		traverseNodeVector(dsgScenegraphPrintLine,NULL);
		return;
	}

}
