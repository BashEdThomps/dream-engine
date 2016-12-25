/*
* ShaderInstance
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ShaderInstance.h"
#include "ShaderCache.h"

namespace Dream {

  ShaderInstance::ShaderInstance(AssetDefinition* definition,Transform3D* transform) : AssetInstance(definition,transform) {
    mShaderProgram = 0;
  }

  ShaderInstance::~ShaderInstance() {
    /*if (mShaderProgram > 0) {
      glDeleteShader(mShaderProgram);
    }
    */
  }

  GLuint ShaderInstance::getShaderProgram() {
    return mShaderProgram;
  }

  bool ShaderInstance::load(string projectPath) {
    mShaderProgram = ShaderCache::getShader(mDefinition->getUuid());
    if (mShaderProgram == 0) {
      string mVertexShaderSource;
      string mFragmentShaderSource;
      GLuint mVertexShader = 0;
      GLuint mFragmentShader = 0;
      // 1. Open Shader Files into Memory
      FileReader *vertexReader, *fragmentReader;
      string absVertexPath, absFragmentPath;
      absVertexPath   = projectPath+mDefinition->getAssetPath() + SHADER_VERTEX;
      absFragmentPath = projectPath+mDefinition->getAssetPath() + SHADER_FRAGMENT;
      vertexReader = new FileReader(absVertexPath);
      vertexReader->readIntoStringStream();
      mVertexShaderSource = vertexReader->getContentsAsString();
      delete vertexReader;
      fragmentReader = new FileReader(absFragmentPath);
      fragmentReader->readIntoStringStream();
      mFragmentShaderSource = fragmentReader->getContentsAsString();
      delete fragmentReader;
      if (DEBUG) {
        cout << "ShaderInstance: Loading Shader..." << endl
             << "ShaderInstance: Vertex: " << absVertexPath     << endl
             << endl << mVertexShaderSource    << endl
             << "ShaderInstance: Fragment: " << absFragmentPath   << endl
             << endl << mFragmentShaderSource  << endl;
      }
      // 2. Compile shaders
      GLint success;
      GLchar infoLog[512];
      // Vertex Shader
      mVertexShader = glCreateShader(GL_VERTEX_SHADER);
      const char *vSource = mVertexShaderSource.c_str();
      glShaderSource(mVertexShader, 1, &vSource, nullptr);
      glCompileShader(mVertexShader);
      // Print compile errors if any
      glGetShaderiv(mVertexShader, GL_COMPILE_STATUS, &success);
      if (!success) {
        glGetShaderInfoLog(mVertexShader, 512, nullptr, infoLog);
        cerr << "ShaderInstance: SHADER:VERTEX:COMPILATION_FAILED\n" << infoLog << endl;
      }
      // Fragment Shader
      mFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
      const char *fSource = mFragmentShaderSource.c_str();
      glShaderSource(mFragmentShader, 1, &fSource, nullptr);
      glCompileShader(mFragmentShader);
      // Print compile errors if any
      glGetShaderiv(mFragmentShader, GL_COMPILE_STATUS, &success);
      if (!success) {
        glGetShaderInfoLog(mFragmentShader, 512, nullptr, infoLog);
        cerr << "ShaderInstance: SHADER:FRAGMENT:COMPILATION_FAILED\n" << infoLog << endl;
      }
      // Shader Program
      mShaderProgram = glCreateProgram();
      glAttachShader(mShaderProgram, mVertexShader);
      glAttachShader(mShaderProgram, mFragmentShader);
      glLinkProgram(mShaderProgram);
      // Print linking errors if any
      glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &success);
      if (!success) {
        glGetProgramInfoLog(mShaderProgram, 512, nullptr, infoLog);
        cerr << "ShaderInstance: SHADER:PROGRAM:LINKING_FAILED\n" << infoLog << endl;
      }
      // Delete the shaders as they're linked into our program now and no longer necessery
      glDeleteShader(mVertexShader);
      glDeleteShader(mFragmentShader);
      ShaderCache::putShader(mDefinition->getUuid(),mShaderProgram);
    }
    mLoaded = (mShaderProgram != 0);
    return mLoaded;
  }

  void ShaderInstance::use() {
    glUseProgram(mShaderProgram);
  }

  void ShaderInstance::loadExtraAttributes(nlohmann::json jsonData) {
    // pass
  }

  // 1f
  void ShaderInstance::setUniform1f(string name, GLfloat val1) {
    glUniform1f(glGetUniformLocation(getShaderProgram(),name.c_str()),val1);
  }

  // 1fv
  void ShaderInstance::setUniform1fv(string name, GLfloat val1) {
    // TODO
  }

  // 1i
  void ShaderInstance::setUniform1i(string name, GLint val1) {
    glUniform1i(glGetUniformLocation(getShaderProgram(),name.c_str()),val1);
  }

  // 1iv
  void ShaderInstance::setUniform1iv(string name, GLint val1) {
    // TODO
  }


  // 2f
  void ShaderInstance::setUniform2f(string name, GLfloat val1, GLfloat val2) {
    glUniform2f(glGetUniformLocation(getShaderProgram(),name.c_str()),val1,val2);
  }

  // 2fv
  void ShaderInstance::setUniform2fv(string name, glm::vec2 value) {
    glUniform2fv(
      glGetUniformLocation(getShaderProgram(), name.c_str()),
      1, glm::value_ptr(value)
    );
  }

  // 2i
  void ShaderInstance::setUniform2i(string name, GLint val1, GLint val2) {
    glUniform2i(glGetUniformLocation(getShaderProgram(),name.c_str()),val1,val2);
  }

  // 2iv
  void ShaderInstance::setUniform2iv(string name, glm::vec2) {
    // TODO
  }


  // 3f
  void ShaderInstance::setUniform3f(string name, GLfloat val1, GLfloat val2, GLfloat val3) {
    glUniform3f(glGetUniformLocation(getShaderProgram(),name.c_str()),val1,val2,val3);
  }

  // 3fv
  void ShaderInstance::setUniform3fv(string name, glm::vec3 value) {
    glUniform3fv(
      glGetUniformLocation(getShaderProgram(), name.c_str()),
      1, glm::value_ptr(value)
    );
  }

  // 3i
  void ShaderInstance::setUniform3i(string name, GLint val1, GLint val2, GLint val3) {
    glUniform3i(glGetUniformLocation(getShaderProgram(),name.c_str()),val1,val2,val3);
  }

  // 3iv
  void ShaderInstance::setUniform3iv(string name, glm::vec3) {
    // TODO
  }


  // 4f
  void ShaderInstance::setUniform4f(string name, GLfloat val1, GLfloat val2, GLfloat val3, GLfloat val4) {
    glUniform4f(glGetUniformLocation(getShaderProgram(),name.c_str()),val1,val2,val3,val4);
  }

  // 4fv
  void ShaderInstance::setUniform4fv(string name, glm::vec4) {
    // TODO
  }

  // 4i
  void ShaderInstance::setUniform4i(string name, GLint val1, GLint val2, GLint val3, GLint val4) {
    glUniform4i(glGetUniformLocation(getShaderProgram(),name.c_str()),val1,val2,val3,val4);
  }

  // 4iv
  void ShaderInstance::setUniform4iv(string name, glm::vec4) {
    // TODO
  }


  // Matrix2fv
  void ShaderInstance::setUniformMatrix2fv(string name, glm::mat2) {
    // TODO
  }

  // Matrix3fv
  void ShaderInstance::setUniformMatrix3fv(string name, glm::mat3) {
    // TODO
  }

  //Matrix4fv
  void ShaderInstance::setUniformMatrix4fv(string name, glm::mat4 value) {
    glUniformMatrix4fv(
      glGetUniformLocation(getShaderProgram(), name.c_str()),
      1, GL_FALSE, glm::value_ptr(value)
    );
  }

} // End of Dream
