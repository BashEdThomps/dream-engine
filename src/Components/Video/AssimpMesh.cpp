#include "AssimpMesh.h"

namespace Dream {
  namespace Components {
    namespace Video {

      AssimpMesh::AssimpMesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures) {
        mVertices = vertices;
        mIndices  = indices;
        mTextures = textures;
        init();
      }

      AssimpMesh::~AssimpMesh() { }

      void AssimpMesh::draw(Dream::Asset::Instances::Shader::ShaderInstance* shader) {
        GLuint diffuseNr = 1;
        GLuint specularNr = 1;
        for(GLuint i = 0; i < mTextures.size(); i++) {
          glActiveTexture(GL_TEXTURE0 + i); // Activate proper texture unit before binding
          // Retrieve texture number (the N in diffuse_textureN)
          std::stringstream ss;
          std::string number;
          std::string name = mTextures[i].type;
          if(name == "texture_diffuse") {
            ss << diffuseNr++; // Transfer GLuint to stream
          } else if(name == "texture_specular") {
            ss << specularNr++; // Transfer GLuint to stream
          }
          number = ss.str();
          glUniform1f(glGetUniformLocation(shader->getShaderProgram(), ("material." + name + number).c_str()), i);
          glBindTexture(GL_TEXTURE_2D, mTextures[i].id);
        }
        glActiveTexture(GL_TEXTURE0);

        // Draw mesh
        glBindVertexArray(mVAO);
        glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
      }

      void AssimpMesh::init() {
        glGenVertexArrays(1, &mVAO);
#ifdef VERBOSE
        checkGLError(1);
#endif

        glGenBuffers(1, &mVBO);
#ifdef VERBOSE
        checkGLError(2);
#endif

        glGenBuffers(1, &mEBO);
#ifdef VERBOSE
        checkGLError(3);
#endif

        glBindVertexArray(mVAO);
#ifdef VERBOSE
        checkGLError(4);
#endif

        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
#ifdef VERBOSE
        checkGLError(5);
#endif

        glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);
#ifdef VERBOSE
        checkGLError(6);
#endif

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
#ifdef VERBOSE
        checkGLError(7);
#endif

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint),&mIndices[0], GL_STATIC_DRAW);
#ifdef VERBOSE
        checkGLError(8);
#endif

        // Vertex Positions
        glEnableVertexAttribArray(0);
#ifdef VERBOSE
        checkGLError(9);
#endif

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
#ifdef VERBOSE
        checkGLError(10);
#endif

        // Vertex Normals
        glEnableVertexAttribArray(1);
#ifdef VERBOSE
        checkGLError(11);
#endif

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
#ifdef VERBOSE
        checkGLError(12);
#endif

        // Vertex Texture Coords
        glEnableVertexAttribArray(2);
#ifdef VERBOSE
        checkGLError(13);
#endif

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
#ifdef VERBOSE
        checkGLError(14);
#endif

        glBindVertexArray(0);
#ifdef VERBOSE
        checkGLError(15);
#endif
      }

      bool AssimpMesh::checkGLError(int errorIndex) {
        GLenum errorCode = 0;
        bool wasError = false;
        do {
          errorCode = glGetError();
          if (errorCode!=0) {
            std::cerr << "Mesh: Error Check " << errorIndex << ": " << std::endl;
            switch (errorCode) {
              case GL_NO_ERROR:
                std::cerr << "\tGL_NO_ERROR" << std::endl;
                break;
              case GL_INVALID_ENUM:
                std::cerr << "\tGL_INVALID_ENUM" << std::endl;
                break;
              case GL_INVALID_VALUE:
                std::cerr << "\tGL_INVALID_VALUE" << std::endl;
                break;
              case GL_INVALID_OPERATION:
                std::cerr << "\tGL_INVALID_OPERATION" << std::endl;
                break;
              case GL_INVALID_FRAMEBUFFER_OPERATION:
                std::cerr << "\tGL_INVALID_FRAMEBUFFER_OPERATION" << std::endl;
                break;
              case GL_OUT_OF_MEMORY:
                std::cerr << "\tGL_OUT_OF_MEMORY" << std::endl;
                break;
            }
            std::cerr << "\tName: " << glewGetErrorString(errorCode) << std::endl;
            std::cerr << "\tCode: " << errorCode << std::endl;
            wasError = true;
          }
        } while(errorCode != 0);
        return wasError;
      }
    }
  }
}
