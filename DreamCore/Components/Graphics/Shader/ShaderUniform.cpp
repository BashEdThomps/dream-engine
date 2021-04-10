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
          mData(nullptr),
          mDataSize(0),
          mCount(count),
          mLocation(0),
          mNeedsUpdate(true)
    {

        LOG_DEBUG("ShaderUniform: Constructing uniform {}, count {}",mName,mCount);
        switch (mType)
        {
            // INT =============================================================
            case UNIFORM_TYPE_INT1:
                mDataSize = sizeof(GLint)*mCount;
                break;
            case UNIFORM_TYPE_INT2:
                mDataSize = sizeof(GLint)*mCount*2;
                break;
            case UNIFORM_TYPE_INT3:
                mDataSize = sizeof(GLint)*mCount*3;
                break;
            case UNIFORM_TYPE_INT4:
                mDataSize = sizeof(GLint)*mCount*4;
                break;
                // UINT ============================================================
            case UNIFORM_TYPE_UINT1:
                mDataSize = sizeof(GLuint)*mCount;
                break;
            case UNIFORM_TYPE_UINT2:
                mDataSize = sizeof(GLuint)*mCount*2;
                break;
            case UNIFORM_TYPE_UINT3:
                mDataSize = sizeof(GLuint)*mCount*3;
                break;
            case UNIFORM_TYPE_UINT4:
                mDataSize = sizeof(GLuint)*mCount*4;
                break;
                // FLOAT ===========================================================
            case UNIFORM_TYPE_FLOAT1:
                mDataSize = sizeof(GLfloat)*mCount;
                break;
            case UNIFORM_TYPE_FLOAT2:
                mDataSize = sizeof(GLfloat)*mCount*2;
                break;
            case UNIFORM_TYPE_FLOAT3:
                mDataSize = sizeof(GLfloat)*mCount*3;
                break;
            case UNIFORM_TYPE_FLOAT4:
                mDataSize = sizeof(GLfloat)*mCount*4;
                break;
                // Matrix ==========================================================
            case UNIFORM_TYPE_MATRIX2:
                mDataSize = sizeof(mat2)*mCount;
                break;
            case UNIFORM_TYPE_MATRIX3:
                mDataSize =  sizeof(mat3)*mCount;
                break;
            case UNIFORM_TYPE_MATRIX4:
                mDataSize = sizeof(mat4)*mCount;
                break;
            case UNIFORM_TYPE_MATRIX2X3:
                mDataSize = sizeof(mat2x3)*mCount;
                break;
            case UNIFORM_TYPE_MATRIX3X2:
                mDataSize = sizeof(mat3x2)*mCount;
                break;
            case UNIFORM_TYPE_MATRIX2X4:
                mDataSize =  sizeof(mat2x4)*mCount;
                break;
            case UNIFORM_TYPE_MATRIX4X2:
                mDataSize = sizeof(mat4x2)*mCount;
                break;
            case UNIFORM_TYPE_MATRIX3X4:
                mDataSize = sizeof(mat3x4)*mCount;
                break;
            case UNIFORM_TYPE_MATRIX4X3:
                mDataSize = sizeof(mat4x3)*mCount;
                break;
        }

        mData = malloc(mDataSize);
        setData(data);
    }

    ShaderUniform::~ShaderUniform()
    {
        LOG_TRACE("ShaderUniform: Destructing {} {}", mName, mCount);
        deleteData();
    }

    void ShaderUniform::deleteData()
    {
        free(mData);
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

    size_t ShaderUniform::getDataSize() const
    {
        return mDataSize;
    }

    void ShaderUniform::setData(void* data)
    {
        // Correct space Allocated
        if (mData != nullptr)
        {
            // Value is not equal to current
            if (memcmp(mData,data, mDataSize) != 0)
            {
				memcpy(mData,data,mDataSize);
				setNeedsUpdate(true);
            }
        }
        else { assert(mData != nullptr); }
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
