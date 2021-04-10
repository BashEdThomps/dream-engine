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
