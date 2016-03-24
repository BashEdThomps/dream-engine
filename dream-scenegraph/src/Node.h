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
		std::string mName;
		std::string mPath;
		int         mParentIndex;
		int         mVertexBufferIndex;
		float       mTranslation;
		float       mRotation;
	public:
		Node(char*);
		~Node();

		void initTranslationRotation();
		int  hasValidName();
		int  hasValidPath();
		void print();
		void setParentIndex(int);
		void setTranslation(float, float, float);
		void setRotation(float, float, float);
		void setPolarTranslation(float, float);
		int  hasVertexBuffer();
	};
}
#endif // NODE_H
