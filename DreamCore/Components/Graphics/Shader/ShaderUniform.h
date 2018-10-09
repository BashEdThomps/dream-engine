#pragma once

#include "../../../Common/DreamObject.h"
#include <GL/glew.h>
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

    class ShaderUniform : public DreamObject
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
