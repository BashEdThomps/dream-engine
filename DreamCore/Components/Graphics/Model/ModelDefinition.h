#pragma once

#include "Components/AssetDefinition.h"

namespace octronic::dream
{
    class ModelDefinition : public AssetDefinition
    {
    public:
        ModelDefinition(ProjectDefinition& pd, const json& js);

        ModelDefinition(ModelDefinition&&) = default;
        ModelDefinition& operator=(ModelDefinition&&) = default;

        bool isFormatAssimp();

        bool addModelMaterial(const string& material, UuidType shader);
        json getModelMaterials() const;
        void removeModelMaterial(const string& material);
        void clearModelMaterialList();
        UuidType getDreamMaterialForModelMaterial(const string& mat);
    };
}
