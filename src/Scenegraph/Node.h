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
		std::string getPath();
		void setPath(std::string);
		bool hasName(std::string);
		std::string getName();
		void print();
		void setParent(Node*);
		void setTranslation(float, float, float);
		void setRotation(float, float, float);
		void setPolarTranslation(float, float);
		Node* getParent();
		bool  isChildOf(Node*);
		bool  isParentOf(Node*);
		int countChildren();
		//void addResource(Dream::Resource*);
	};
}
#endif // NODE_H
