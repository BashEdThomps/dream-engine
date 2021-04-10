#pragma once

#include "Components/AssetDefinition.h"

namespace octronic::dream
{
    class FontDefinition : public AssetDefinition
    {
    public:
        FontDefinition(ProjectDefinition& pd, const json& js);
        FontDefinition(FontDefinition&&) = default;
        FontDefinition& operator=(FontDefinition&&) = default;

        void setSize(unsigned int size);
        unsigned int getSize() const;
    };
}

