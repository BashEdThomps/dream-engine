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

namespace Dream
{
  ShaderUniform::ShaderUniform(UniformType type, string name, int count, void* data)
        : DreamObject ("ShaderUniform"),
          mType(type),
          mName(name),
          mCount(count),
          mLocation(0),
          mNeedsUpdate(true)
    {
#ifdef DREAM_LOG
        getLog()->debug("Constructing uniform {}, count {}",mName,count);
#endif
        switch (type)
        {
            case Dream::INT1:
                mData = new GLint[count];
                memcpy(mData,data,sizeof(GLint)*static_cast<unsigned long>(count));
                break;
            case Dream::INT2:
                mData = new ivec2[count];
                memcpy(mData,data,sizeof(ivec2)*static_cast<unsigned long>(count));
                break;
            case Dream::INT3:
                mData = new ivec3[count];
                memcpy(mData,data,sizeof(ivec3)*static_cast<unsigned long>(count));
                break;
            case Dream::INT4:
                mData = new ivec4[count];
                memcpy(mData,data,sizeof(ivec4)*static_cast<unsigned long>(count));
                break;

            case Dream::UINT1:
                mData = new GLint[count];
                memcpy(mData,data,sizeof(GLuint)*static_cast<unsigned long>(count));
                break;
            case Dream::UINT2:
                mData = new uvec2[count];
                memcpy(mData,data,sizeof(uvec2)*static_cast<unsigned long>(count));
                break;
            case Dream::UINT3:
                mData = new uvec3[count];
                memcpy(mData,data,sizeof(uvec3)*static_cast<unsigned long>(count));
                break;
            case Dream::UINT4:
                mData = new uvec4[count];
                memcpy(mData,data,sizeof(uvec4)*static_cast<unsigned long>(count));
                break;

            case Dream::FLOAT1:
                mData = new GLint[count];
                memcpy(mData,data,sizeof(GLfloat)*static_cast<unsigned long>(count));
                break;
            case Dream::FLOAT2:
                mData = new vec2[count];
                memcpy(mData,data,sizeof(vec2)*static_cast<unsigned long>(count));
                break;
            case Dream::FLOAT3:
                mData = new vec3[count];
                memcpy(mData,data,sizeof(vec3)*static_cast<unsigned long>(count));
                break;
            case Dream::FLOAT4:
                mData = new vec4[count];
                memcpy(mData,data,sizeof(vec4)*static_cast<unsigned long>(count));
                break;
        }
    }

    ShaderUniform::~ShaderUniform()
    {
#ifdef DREAM_LOG
        auto log = getLog();
        log->trace("Destructing {} {}", mName, mCount);
#endif
        switch (mType)
        {
            case Dream::INT1:
                delete[] static_cast<GLint*>(mData);
                break;
            case Dream::INT2:
                delete[] static_cast<ivec2*>(mData);
                break;
            case Dream::INT3:
                delete[] static_cast<ivec3*>(mData);
                break;
            case Dream::INT4:
                delete[] static_cast<ivec4*>(mData);
                break;

            case Dream::UINT1:
                delete[] static_cast<GLuint*>(mData);
                break;
            case Dream::UINT2:
                delete[] static_cast<uvec2*>(mData);
                break;
            case Dream::UINT3:
                delete[] static_cast<uvec3*>(mData);
                break;
            case Dream::UINT4:
                delete[] static_cast<uvec4*>(mData);
                break;

            case Dream::FLOAT1:
                delete[] static_cast<GLfloat*>(mData);
                break;
            case Dream::FLOAT2:
                delete[] static_cast<vec2*>(mData);
                break;
            case Dream::FLOAT3:
                delete[] static_cast<vec3*>(mData);
                break;
            case Dream::FLOAT4:
                delete[] static_cast<vec4*>(mData);
                break;
        }
    }

    bool
    ShaderUniform::operator==
    (const ShaderUniform& other) const
    {
        return getName() == other.getName();
    }

    int ShaderUniform::getCount() const
    {
        return mCount;
    }

    void ShaderUniform::setCount(int count)
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
            case Dream::INT1:
                size = sizeof(GLint)*static_cast<unsigned long>(mCount);
                break;
            case Dream::INT2:
                size = sizeof(ivec2)*static_cast<unsigned long>(mCount);
                break;
            case Dream::INT3:
                size = sizeof(ivec3)*static_cast<unsigned long>(mCount);
                break;
            case Dream::INT4:
                size = sizeof(ivec4)*static_cast<unsigned long>(mCount);
                break;

            case Dream::UINT1:
                size = sizeof(GLuint)*static_cast<unsigned long>(mCount);
                break;
            case Dream::UINT2:
                size = sizeof(uvec2)*static_cast<unsigned long>(mCount);
                break;
            case Dream::UINT3:
                size = sizeof(uvec3)*static_cast<unsigned long>(mCount);
                break;
            case Dream::UINT4:
                size = sizeof(uvec4)*static_cast<unsigned long>(mCount);
                break;

            case Dream::FLOAT1:
                size = sizeof(GLfloat)*static_cast<unsigned long>(mCount);
                break;
            case Dream::FLOAT2:
                size = sizeof(vec2)*static_cast<unsigned long>(mCount);
                break;
            case Dream::FLOAT3:
                size = sizeof(vec3)*static_cast<unsigned long>(mCount);
                break;
            case Dream::FLOAT4:
                size = sizeof(vec4)*static_cast<unsigned long>(mCount);
                break;
        }

        if (size > 0)
        {
            if (memcmp(mData,data,size) == 0) return;
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
