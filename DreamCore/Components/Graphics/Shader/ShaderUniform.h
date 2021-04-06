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
#include "UniformType.h"
#include <glm/glm.hpp>

namespace octronic::dream
{
    class ShaderUniform
    {
    public:
        ShaderUniform(UniformType type, string name, size_t count, void* data);

        ShaderUniform(ShaderUniform&) = delete;
        ShaderUniform& operator=(ShaderUniform&) = delete;

        ShaderUniform(ShaderUniform&&) = default;
        ShaderUniform& operator=(ShaderUniform&&) = default;

        ~ShaderUniform();

        bool operator==(const ShaderUniform& other) const;

        UniformType getType() const;
        void setType(const UniformType& type);

        string getName() const;
        void setName(const string& name);

        void* getData() const;
        void setData(void* data);

        size_t getCount() const;
        void setCount(size_t count);

        GLint getLocation() const;
        void setLocation(GLint location);

        bool getNeedsUpdate() const;
        void setNeedsUpdate(bool needsUpdate);

        size_t getDataSize() const;

    private:
        void deleteData();

    private:
        UniformType mType;
        string      mName;
        void*       mData;
        size_t      mDataSize;
        size_t      mCount;
        GLint       mLocation;
        bool        mNeedsUpdate;
    };
}
