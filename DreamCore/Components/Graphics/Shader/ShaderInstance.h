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
#include "../../../Common/ILoggable.h"
#include "../../IAssetInstance.h"

using std::string;
using nlohmann::json;
using glm::mat4;
using glm::vec3;



namespace Dream
{
    class ShaderCache;
    class ShaderDefinition;

    enum UniformType
    {
        // Int
        INT1,
        INT2,
        INT3,
        INT4,
        // uInt
        UINT1,
        UINT2,
        UINT3,
        UINT4,
        // Float
        FLOAT1,
        FLOAT2,
        FLOAT3,
        FLOAT4,
        // Double
        DOUBLE1,
        DOUBLE2,
        DOUBLE3,
        DOUBLE4
    };

    class ShaderUniform : ILoggable
    {
    public:
        ShaderUniform(UniformType type, string name, int count, void* data);
        ~ShaderUniform() override;

        bool operator==(const ShaderUniform& other) const;

        UniformType getType() const;
        void setType(const UniformType& type);
        string getName() const;
        void setName(const string& name);
        void* getData() const;
        void setData(void* data);
        int getCount() const;
        void setCount(int count);

    private:
        UniformType mType;
        string mName;
        void* mData;
        int mCount;

    };


    class ShaderInstance : public IAssetInstance, ILoggable
    {
    private:
        const static GLint UNIFORM_NOT_FOUND;
        GLuint mShaderProgram;
        vector<ShaderUniform> mUniformVector;
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

        void bindVertexArray(GLuint);
        void unbindVertexArray();

        GLint getUniformLocation(string name);

        void addUniform(ShaderUniform uniform);

        void syncUniforms();
    }; // End of ShaderInstance

} // End of Dream
