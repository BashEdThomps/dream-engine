#pragma once

#include "Components/AssetDefinition.h"

namespace octronic::dream
{
    class ScriptDefinition : public AssetDefinition
    {
    public:
        ScriptDefinition(ProjectDefinition& pd, const json& js);

        ScriptDefinition(ScriptDefinition&&) = default;
        ScriptDefinition& operator=(ScriptDefinition&&) = default;

        bool isFormatLua();
    };
}
