#pragma once

#include "Components/AssetDefinition.h"

namespace octronic::dream
{

    class MaterialDefinition : public AssetDefinition
    {
    public:
        MaterialDefinition(ProjectDefinition& pd, const json& js);

				MaterialDefinition(MaterialDefinition&&) = default;
        MaterialDefinition& operator=(MaterialDefinition&&) = default;

        // Shader
        UuidType getShaderUuid() const;
        void setShaderUuid(UuidType);

        // Textures
        UuidType getAlbedoTextureUuid() const;
        void setAlbedoTextureUuid(UuidType);

        UuidType getNormalTextureUuid() const;
        void setNormalTextureUuid(UuidType);

        UuidType getMetallicTextureUuid() const;
        void setMetallicTextureUuid(UuidType);

        UuidType getRoughnessTextureUuid() const;
        void setRoughnessTextureUuid(UuidType);

        UuidType getAoTextureUuid() const;
        void setAoTextureUuid(UuidType);
    };
}

