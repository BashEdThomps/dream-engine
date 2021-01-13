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


#pragma once

#include "Common/GLHeader.h"
#include <glm/glm.hpp>

using namespace glm;

namespace Dream
{
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

    class ShaderUniform
    {
    public:
        ShaderUniform(UniformType type, string name, int count, void* data);
        ShaderUniform(const ShaderUniform&) = delete;
        ~ShaderUniform();

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

        bool getNeedsUpdate() const;
        void setNeedsUpdate(bool needsUpdate);

    private:
        UniformType mType;
        string mName;
        void* mData;
        int mCount;
        GLint mLocation;
        bool mNeedsUpdate;
    };
}
