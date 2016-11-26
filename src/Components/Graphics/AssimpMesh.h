#ifndef AssimpMesh_h
#define AssimpMesh_h

#include <iostream>
#include <sstream>
#include <vector>


#include <GL/glew.h>
#include <glm/glm.hpp>
#include <assimp/types.h>

#include "ShaderInstance.h"

namespace Dream {

      struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
      };

      struct Texture {
        GLuint id;
        string type;
        aiString path;
      };

      class AssimpMesh {
      private:
        GLuint mVAO;
        GLuint mVBO;
        GLuint mEBO;
        vector<Vertex>  mVertices;
        vector<GLuint>  mIndices;
        vector<Texture> mTextures;
      public:
        AssimpMesh(vector<Vertex>, vector<GLuint>, vector<Texture>);
        ~AssimpMesh();
        void draw(ShaderInstance*);
        void init();
        bool checkGLError(int);
      };
    }

#endif /* AssimpMesh_h */
