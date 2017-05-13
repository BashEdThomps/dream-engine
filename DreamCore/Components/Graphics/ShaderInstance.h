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
#include "../../IAssetInstance.h"

#include <vector>
#include <map>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>

#define SHADER_VERTEX   "vertex"
#define SHADER_FRAGMENT "fragment"

using namespace std;

namespace Dream
{
    class ShaderInstance : public IAssetInstance
    {
    private:
        GLuint mShaderProgram;
        map<string, GLfloat>  mUniform1fMap;
    public:
        ShaderInstance();
        ShaderInstance(AssetDefinition*,Transform3D*);
        ~ShaderInstance();
        bool load(string);
        void use();
        void syncUniforms();
        GLuint getShaderProgram();
        void loadExtraAttributes(nlohmann::json);
        void exposeLuaApi(lua_State* state);
    public:
        // 1f
        void setUniform1f(string, GLfloat);
    private:
        // 1f
        void syncUniform1f();
    }; // End of ShaderInstance
} // End of Dream

#endif // End of SHADERINSTANCE_H
