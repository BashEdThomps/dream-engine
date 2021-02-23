/*
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
#include "ShaderUniform.h"
#include "Common/Logger.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::ivec2;
using glm::ivec3;
using glm::ivec4;
using glm::uvec2;
using glm::uvec3;
using glm::uvec4;
using glm::mat2;
using glm::mat3;
using glm::mat4;
using glm::mat2x3;
using glm::mat2x4;
using glm::mat3x2;
using glm::mat3x4;
using glm::mat4x2;
using glm::mat4x3;

namespace octronic::dream
{
    ShaderUniform::ShaderUniform(UniformType type, string name, size_t count, void* data)
        : mType(type),
          mName(name),
          mCount(count),
          mLocation(0),
          mData(nullptr),
          mNeedsUpdate(true)
    {

        LOG_DEBUG("ShaderUniform: Constructing uniform {}, count {}",mName,count);
        switch (type)
        {
            // INT =============================================================
            case UNIFORM_TYPE_INT1:
                mData = new GLint[count];
                memcpy(mData,data,sizeof(GLint)*count);
                break;
            case UNIFORM_TYPE_INT2:
                mData = new ivec2[count];
                memcpy(mData,data,sizeof(ivec2)*count);
                break;
            case UNIFORM_TYPE_INT3:
                mData = new ivec3[count];
                memcpy(mData,data,sizeof(ivec3)*count);
                break;
            case UNIFORM_TYPE_INT4:
                mData = new ivec4[count];
                memcpy(mData,data,sizeof(ivec4)*count);
                break;
                // UINT ============================================================
            case UNIFORM_TYPE_UINT1:
                mData = new GLuint[count];
                memcpy(mData,data,sizeof(GLuint)*count);
                break;
            case UNIFORM_TYPE_UINT2:
                mData = new uvec2[count];
                memcpy(mData,data,sizeof(uvec2)*count);
                break;
            case UNIFORM_TYPE_UINT3:
                mData = new uvec3[count];
                memcpy(mData,data,sizeof(uvec3)*count);
                break;
            case UNIFORM_TYPE_UINT4:
                mData = new uvec4[count];
                memcpy(mData,data,sizeof(uvec4)*count);
                break;
                // FLOAT ===========================================================
            case UNIFORM_TYPE_FLOAT1:
                mData = new GLint[count];
                memcpy(mData,data,sizeof(GLfloat)*count);
                break;
            case UNIFORM_TYPE_FLOAT2:
                mData = new vec2[count];
                memcpy(mData,data,sizeof(vec2)*count);
                break;
            case UNIFORM_TYPE_FLOAT3:
                mData = new vec3[count];
                memcpy(mData,data,sizeof(vec3)*count);
                break;
            case UNIFORM_TYPE_FLOAT4:
                mData = new vec4[count];
                memcpy(mData,data,sizeof(vec4)*count);
                break;
            // Matrix ==========================================================
            case UNIFORM_TYPE_MATRIX2:
                mData = new mat2[count];
                memcpy(mData,data,sizeof(mat2)*count);
                break;
            case UNIFORM_TYPE_MATRIX3:
                mData = new mat3[count];
                memcpy(mData,data,sizeof(mat3)*count);
                break;
            case UNIFORM_TYPE_MATRIX4:
                mData = new mat4[count];
                memcpy(mData,data,sizeof(mat4)*count);
                break;
            case UNIFORM_TYPE_MATRIX2X3:
                mData = new mat2x3[count];
                memcpy(mData,data,sizeof(mat2x3)*count);
                break;
            case UNIFORM_TYPE_MATRIX3X2:
                mData = new mat3x2[count];
                memcpy(mData,data,sizeof(mat3x2)*count);
                break;
            case UNIFORM_TYPE_MATRIX2X4:
                mData = new mat2x4[count];
                memcpy(mData,data,sizeof(mat2x4)*count);
                break;
            case UNIFORM_TYPE_MATRIX4X2:
                mData = new mat4x2[count];
                memcpy(mData,data,sizeof(mat4x2)*count);
                break;
            case UNIFORM_TYPE_MATRIX3X4:
                mData = new mat3x4[count];
                memcpy(mData,data,sizeof(mat3x4)*count);
                break;
            case UNIFORM_TYPE_MATRIX4X3:
                mData = new mat4x3[count];
                memcpy(mData,data,sizeof(mat4x3)*count);
                break;
        }
    }

    ShaderUniform::~ShaderUniform()
    {
        LOG_TRACE("ShaderUniform: Destructing {} {}", mName, mCount);
        deleteData();
    }

	void ShaderUniform::deleteData()
    {
        switch (mType)
        {
            // INT =============================================================
            case UNIFORM_TYPE_INT1:
                delete[] static_cast<GLint*>(mData);
                break;
            case UNIFORM_TYPE_INT2:
                delete[] static_cast<ivec2*>(mData);
                break;
            case UNIFORM_TYPE_INT3:
                delete[] static_cast<ivec3*>(mData);
                break;
            case UNIFORM_TYPE_INT4:
                delete[] static_cast<ivec4*>(mData);
                break;
                // UINT ============================================================
            case UNIFORM_TYPE_UINT1:
                delete[] static_cast<GLuint*>(mData);
                break;
            case UNIFORM_TYPE_UINT2:
                delete[] static_cast<uvec2*>(mData);
                break;
            case UNIFORM_TYPE_UINT3:
                delete[] static_cast<uvec3*>(mData);
                break;
            case UNIFORM_TYPE_UINT4:
                delete[] static_cast<uvec4*>(mData);
                break;
                // FLOAT ===========================================================
            case UNIFORM_TYPE_FLOAT1:
                delete[] static_cast<GLfloat*>(mData);
                break;
            case UNIFORM_TYPE_FLOAT2:
                delete[] static_cast<vec2*>(mData);
                break;
            case UNIFORM_TYPE_FLOAT3:
                delete[] static_cast<vec3*>(mData);
                break;
            case UNIFORM_TYPE_FLOAT4:
                delete[] static_cast<vec4*>(mData);
                break;
                // Matrix ==========================================================
            case UNIFORM_TYPE_MATRIX2:
                delete[] static_cast<mat2*>(mData);
                break;
            case UNIFORM_TYPE_MATRIX3:
                delete[] static_cast<mat3*>(mData);
                break;
            case UNIFORM_TYPE_MATRIX4:
                delete[] static_cast<mat4*>(mData);
                break;
            case UNIFORM_TYPE_MATRIX2X3:
                delete[] static_cast<mat2x3*>(mData);
                break;
            case UNIFORM_TYPE_MATRIX3X2:
                delete[] static_cast<mat3x2*>(mData);
                break;
            case UNIFORM_TYPE_MATRIX2X4:
                delete[] static_cast<mat2x4*>(mData);
                break;
            case UNIFORM_TYPE_MATRIX4X2:
                delete[] static_cast<mat4x2*>(mData);
                break;
            case UNIFORM_TYPE_MATRIX3X4:
                delete[] static_cast<mat3x4*>(mData);
                break;
            case UNIFORM_TYPE_MATRIX4X3:
                delete[] static_cast<mat4x3*>(mData);
                break;
        }
        mData = nullptr;
    }

    bool
    ShaderUniform::operator==
    (const ShaderUniform& other) const
    {
        return getName() == other.getName();
    }

    size_t ShaderUniform::getCount() const
    {
        return mCount;
    }

    void ShaderUniform::setCount(size_t count)
    {
        mCount = count;
        setNeedsUpdate(true);
    }

    GLint ShaderUniform::getLocation() const
    {
        return mLocation;
    }

    void ShaderUniform::setLocation(GLint location)
    {
        mLocation = location;
        setNeedsUpdate(true);
    }

    bool ShaderUniform::getNeedsUpdate() const
    {
        return mNeedsUpdate;
    }

    void ShaderUniform::setNeedsUpdate(bool needsUpdate)
    {
        mNeedsUpdate = needsUpdate;
    }

    void* ShaderUniform::getData() const
    {
        return mData;
    }

    void ShaderUniform::setData(void* data)
    {
        size_t size = 0;
        switch (mType)
        {
            // INT =============================================================
            case UNIFORM_TYPE_INT1:
                size = sizeof(GLint)*mCount;
                break;
            case UNIFORM_TYPE_INT2:
                size = sizeof(ivec2)*mCount;
                break;
            case UNIFORM_TYPE_INT3:
                size = sizeof(ivec3)*mCount;
                break;
            case UNIFORM_TYPE_INT4:
                size = sizeof(ivec4)*mCount;
                break;
                // UINT ============================================================
            case UNIFORM_TYPE_UINT1:
                size = sizeof(GLuint)*mCount;
                break;
            case UNIFORM_TYPE_UINT2:
                size = sizeof(uvec2)*mCount;
                break;
            case UNIFORM_TYPE_UINT3:
                size = sizeof(uvec3)*mCount;
                break;
            case UNIFORM_TYPE_UINT4:
                size = sizeof(uvec4)*mCount;
                break;
                // FLOAT ===========================================================
            case UNIFORM_TYPE_FLOAT1:
                size = sizeof(GLfloat)*mCount;
                break;
            case UNIFORM_TYPE_FLOAT2:
                size = sizeof(vec2)*mCount;
                break;
            case UNIFORM_TYPE_FLOAT3:
                size = sizeof(vec3)*mCount;
                break;
            case UNIFORM_TYPE_FLOAT4:
                size = sizeof(vec4)*mCount;
                break;
                // Matrix ==========================================================
            case UNIFORM_TYPE_MATRIX2:
                size = sizeof(mat2)*mCount;
                break;
            case UNIFORM_TYPE_MATRIX3:
                size = sizeof(mat3)*mCount;
                break;
            case UNIFORM_TYPE_MATRIX4:
                size = sizeof(mat4)*mCount;
                break;
            case UNIFORM_TYPE_MATRIX2X3:
                size = sizeof(mat2x3)*mCount;
                break;
            case UNIFORM_TYPE_MATRIX3X2:
                size = sizeof(mat3x2)*mCount;
                break;
            case UNIFORM_TYPE_MATRIX2X4:
                size = sizeof(mat2x4)*mCount;
                break;
            case UNIFORM_TYPE_MATRIX4X2:
                size = sizeof(mat4x2)*mCount;
                break;
            case UNIFORM_TYPE_MATRIX3X4:
                size = sizeof(mat3x4)*mCount;
                break;
            case UNIFORM_TYPE_MATRIX4X3:
                size = sizeof(mat4x3)*mCount;
                break;
        }

        // Space Allocated & not equal to current
        if (mData != nullptr && memcmp(mData,data, size) != 0)
        {
            memcpy(mData,data,size);
            setNeedsUpdate(true);
        }
    }

    string
    ShaderUniform::getName
    () const
    {
        return mName;
    }

    void
    ShaderUniform::setName
    (const string& name)
    {
        mName = name;
        setNeedsUpdate(true);
    }

    UniformType
    ShaderUniform::getType
    () const
    {
        return mType;
    }

    void
    ShaderUniform::setType
    (const UniformType& type)
    {
        mType = type;
        setNeedsUpdate(true);
    }
}
