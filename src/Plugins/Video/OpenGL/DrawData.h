#ifndef DrawData_hpp
#define DrawData_hpp

#include "../../../Asset/Instances/Shader/ShaderInstance.h"
#include <glfw/glfw3.h>

namespace Dream   {
namespace Plugins {
namespace Video   {
namespace OpenGL  {
	
	class DrawData {
	public:
		DrawData();
		~DrawData();
		
		GLuint mVertexBuffer;
		size_t mVertexBufferSize;
		
		GLuint mIndexBuffer;
		size_t mIndexBufferSize;
		
		GLuint mVertexArrayObject;
		
		Asset::Instances::Shader::ShaderInstance* mShaderInstance;
	};
	
}
}
}
}

#endif /* DrawData_hpp */
