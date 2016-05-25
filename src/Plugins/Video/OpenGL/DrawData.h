#ifndef DrawData_hpp
#define DrawData_hpp

#include <glfw/glfw3.h>
#include "../../../Asset/Instances/Shader/ShaderInstance.h"

namespace Dream   {
namespace Plugins {
namespace Video   {
namespace OpenGL  {
	
	class DrawData {
	public:
		DrawData();
		~DrawData();
		
		GLuint mVertexDraw;
		size_t mVertexDrawSize;
		
		GLuint mIndexDraw;
		size_t mIndexDrawSize;
		GLuint mVertexArrayObject;
		Dream::Assets::Instances::Shader* mShaderInstance;
	};
	
}
}
}
}

#endif /* DrawData_hpp */
