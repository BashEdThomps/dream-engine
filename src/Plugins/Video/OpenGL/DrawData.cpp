#include "DrawData.h"

namespace Dream   {
namespace Plugins {
namespace Video   {
namespace OpenGL  {
	
	DrawData::DrawData() {
		mVertexArrayObject = 0;
		mVertexBuffer      = 0;
		mVertexBufferSize  = 0;
		mIndexBuffer       = 0;
		mIndexBufferSize   = 0;
		mShaderInstance    = NULL;
	}
	
	DrawData::~DrawData() {}
	
}
}
}
}