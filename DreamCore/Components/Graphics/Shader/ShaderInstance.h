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
    class AssimpMaterial;
    class LightInstance;

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
    };

    class ShaderUniform : ILoggable
    {
    public:
        ShaderUniform(UniformType type, string name, int count, void* data);
        ShaderUniform(const ShaderUniform&) = delete;
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

        GLint getLocation() const;
        void setLocation(GLint location);

    private:
        UniformType mType;
        string mName;
        void* mData;
        int mCount;
        GLint mLocation;

    };


    class ShaderInstance : public IAssetInstance, ILoggable
    {
    private:
        const static unsigned int MAX_LIGHTS;
        const static GLint UNIFORM_NOT_FOUND;
        const static char* UNIFORM_POINT_LIGHT_COUNT;
        const static char* UNIFORM_SPOT_LIGHT_COUNT;
        const static char* UNIFORM_DIRECTIONAL_LIGHT_COUNT;

        int mPointLightCount;
        GLint mPointLightCountLocation;

        int mSpotLightCount;
        GLint mSpotLightCountLocation;

        int mDirectionalLightCount;
        GLint mDirectionalLightCountLocation;

        GLuint mShaderProgram;

        vector<shared_ptr<ShaderUniform>> mUniformVector;
        ShaderCache *mCacheHandle;
    public:
        ShaderInstance(ShaderCache* cache, ShaderDefinition*,SceneObjectRuntime*);
        ~ShaderInstance();


        bool load(string);
        void loadExtraAttributes(json);

        void use();
        void unbind();
        GLuint getShaderProgram();

        // MVP
        bool setModelMatrix(mat4,string name = "model");
        bool setViewMatrix(mat4,string name = "view");
        bool setProjectionMatrix(mat4,string name = "projection");

        // Viewer
        bool setViewerPosition(vec3, string name = "viewPos");

        void bindVertexArray(GLuint);
        void unbindVertexArray();

        GLint getUniformLocation(string name);

        void addUniform(UniformType type, string name, int count, void* data);
        void bindLight(LightInstance* light);

        void syncUniforms();
        void bindMaterial(AssimpMaterial* material);
    }; // End of ShaderInstance

} // End of Dream
