#pragma once

#include "Components/AssetDefinition.h"

namespace octronic::dream
{
    class TextureDefinition : public AssetDefinition
    {
    public:
        TextureDefinition(ProjectDefinition& pd, const json& js);

        TextureDefinition(TextureDefinition&&) = default;
        TextureDefinition& operator=(TextureDefinition&&) = default;

        bool getIsEnvironmentTexture() const;
        void setIsEnvironmentTexture(bool b);

        bool getFlipVertical() const;
        void setFlipVertical(bool b);

        UuidType getEquiToCubeMapShader() const;
        void setEquiToCubeMapShader(UuidType u);

        UuidType getIrradianceMapShader() const;
        void setIrradianceMapShader(UuidType u);

        UuidType getPreFilterShader() const;
        void setPreFilterShader(UuidType u);

        UuidType getBrdfLutShader() const;
        void setBrdfLutShader(UuidType u);
    };
}

