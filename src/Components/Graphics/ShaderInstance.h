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

#ifndef SHADERINSTANCE_H
#define SHADERINSTANCE_H

#include "../../Constants.h"
#include "../../AssetInstance.h"

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>

#define SHADER_VERTEX   "vertex"
#define SHADER_FRAGMENT "fragment"

namespace Dream {
    using namespace std;
      class ShaderInstance : public AssetInstance {
      private:
        GLuint mShaderProgram;
      public:
        ShaderInstance();
        ShaderInstance(AssetDefinition*,Transform3D*);
        ~ShaderInstance();
        bool load(string);
        void use();
        GLuint getShaderProgram();
        void loadExtraAttributes(nlohmann::json);

        // 1f
        void setUniform1f(string, GLfloat);
        // 1fv
        void setUniform1fv(string, GLfloat);
        // 1i
        void setUniform1i(string, GLint);
        // 1iv
        void setUniform1iv(string, GLint);

        // 2f
        void setUniform2f(string, GLfloat, GLfloat);
        // 2fv
        void setUniform2fv(string, glm::vec2);
        // 2i
        void setUniform2i(string, GLint, GLint);
        // 2iv
        void setUniform2iv(string, glm::vec2);

        // 3f
        void setUniform3f(string, GLfloat, GLfloat, GLfloat);
        // 3fv
        void setUniform3fv(string, glm::vec3);
        // 3i
        void setUniform3i(string, GLint, GLint, GLint);
        // 3iv
        void setUniform3iv(string, glm::vec3);

        // 4f
        void setUniform4f(string, GLfloat, GLfloat, GLfloat, GLfloat);
        // 4fv
        void setUniform4fv(string, glm::vec4);
        // 4i
        void setUniform4i(string, GLint, GLint, GLint, GLint);
        // 4iv
        void setUniform4iv(string, glm::vec4);

        // Matrix2fv
        void setUniformMatrix2fv(string, glm::mat2);
        // Matrix3fv
        void setUniformMatrix3fv(string, glm::mat3);
        //Matrix4fv
        void setUniformMatrix4fv(string, glm::mat4);

      }; // End of ShaderInstance

} // End of Dream

#endif // End of SHADERINSTANCE_H
