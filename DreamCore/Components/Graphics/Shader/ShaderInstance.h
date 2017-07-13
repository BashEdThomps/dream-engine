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


#pragma once

#include <vector>
#include <map>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>

#include "../../../Common/Constants.h"
#include "../../IAssetInstance.h"

using std::string;
using nlohmann::json;
using glm::mat4;
using glm::vec3;

namespace Dream
{
    class ShaderCache;
    class ShaderDefinition;

    class ShaderInstance : public IAssetInstance
    {
    private:
        const static GLint UNIFORM_NOT_FOUND;
        GLuint mShaderProgram;
        map<string, GLfloat> mUniform1fMap;
        ShaderCache *mCacheHandle;
    public:
        ShaderInstance(ShaderCache* cache, ShaderDefinition*,SceneObjectRuntime*);
        ~ShaderInstance();

        bool load(string);
        void loadExtraAttributes(json);

        void use();
        void unbind();
        GLuint getShaderProgram();

        // Diffuse & Specular
        bool setDiffuseColour(vec3 diffuse, string name = "diffuseColour");
        bool setSpecularColour(vec3 specular, string name = "specularColour");

        // Point Light
        bool setPointLight(int index, vec3 position, vec3 colour);
        bool setPointLightColour(int index, vec3 colour);
        bool setPointLightPosition(int index, vec3 position);

        // Ambient Light
        bool setAmbientLight(vec3,float);
        bool setAmbientColour(vec3,string name = "ambientColour");
        bool setAmbientStrength(float,string name = "ambientStrength");

        // MVP
        bool setModelMatrix(mat4,string name = "model");
        bool setViewMatrix(mat4,string name = "view");
        bool setProjectionMatrix(mat4,string name = "projection");

        // Viewer
        bool setViewerPosition(vec3, string name = "viewPos");

        // Other Uniforms
        void setUniform1f(string, GLfloat);
        void syncUniforms();

        void bindVertexArray(GLuint);
        void unbindVertexArray();

        GLint getUniformLocation(string name);

    private:
        // 1f
        void syncUniform1f();
    }; // End of ShaderInstance

} // End of Dream
