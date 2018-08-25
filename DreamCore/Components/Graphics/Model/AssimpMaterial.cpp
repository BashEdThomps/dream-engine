#include "AssimpMaterial.h"
#include "AssimpMesh.h"

namespace Dream
{
    bool AssimpMaterial::operator==(const AssimpMaterial& other)
    {
        return mName.operator==(other.mName);
    }
}
