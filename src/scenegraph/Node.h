/*
* node.h
*/
#ifndef NODE_H
#define NODE_H

#define NODE_X 0
#define NODE_Y 1
#define NODE_Z 2

namespace DreamScenegraph {
	class Node {
	protected:
		Node*       mParent;
		std::string mName;
		std::string mPath;
		float       mTranslation[3];
		float       mRotation[3];
	public:
		Node();
		Node(std::string);
		~Node();
		void init();
		void initTranslationRotation();
		int  hasValidName();
		int  hasValidPath();
		bool hasName(std::string);
		std::string getName();
		void print();
		void setParentIndex(int);
		void setTranslation(float, float, float);
		void setRotation(float, float, float);
		void setPolarTranslation(float, float);
		int  hasVertexBuffer();
		int countChildren();
		void addResource(Resource*);
	};
}
#endif // NODE_H
