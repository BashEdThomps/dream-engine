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

#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include "../../AssetInstance.h"

#define SHADER_VERTEX   "vertex"
#define SHADER_FRAGMENT "fragment"

namespace Dream {

      class ShaderInstance : public AssetInstance {
      private:
        string mVertexShaderSource;
        GLuint      mVertexShader;
        string mFragmentShaderSource;
        GLuint      mFragmentShader;
        GLuint      mShaderProgram;
      public:
        ShaderInstance(AssetDefinition*,Transform3D*);
        ~ShaderInstance(void);

        bool load(string);
        void use();
        GLuint getShaderProgram();
        void loadExtraAttributes(nlohmann::json);

        void setUniformInt(string,int);
        void setUniformVector2(string,glm::vec2);
        void setUniformVector3(string,glm::vec3);
        void setUniformMatrix4(string,glm::mat4);
      }; // End of ShaderInstance

} // End of Dream

#endif // End of SHADERINSTANCE_H
