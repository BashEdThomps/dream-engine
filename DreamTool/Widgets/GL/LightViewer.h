#pragma once
#include "GLWidget.h"

namespace DreamTool
{ 
	class LightViewer :
		public GLWidget
	{
	public:
		LightViewer(Project* project);
		~LightViewer();
	protected:
		const static vector<GLfloat> LightModelVertices;
		const static vector<GLuint> LightModelIndices;
	};
}

