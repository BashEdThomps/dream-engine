#ifndef AssimpMesh_h
#define AssimpMesh_h

#include <iostream>
#include <sstream>
#include <vector>


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <assimp/types.h>

#include "../../../../Asset/Instances/Shader/ShaderInstance.h"

namespace Dream      {
namespace Asset      {
namespace Instances  {
namespace Model      {
namespace Assimp     {
	
	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};
	
	struct Texture {
		GLuint id;
		std::string type;
		aiString path;
	};
	
	class Mesh {
	private:
		GLuint mVAO;
		GLuint mVBO;
		GLuint mEBO;
		std::vector<Vertex>  mVertices;
		std::vector<GLuint>  mIndices;
		std::vector<Texture> mTextures;
	public:
		Mesh(std::vector<Vertex>, std::vector<GLuint>, std::vector<Texture>);
		~Mesh();
		void draw(Dream::Asset::Instances::Shader::ShaderInstance*);
		void init();
		bool checkGLError(int);
	};
	
}
}
}
}
}

#endif /* AssimpMesh_h */
