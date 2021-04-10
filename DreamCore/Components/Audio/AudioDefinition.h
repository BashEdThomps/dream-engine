#pragma once

#include "Components/AssetDefinition.h"

namespace octronic::dream
{
    class AudioDefinition : public AssetDefinition
    {
    public:
        AudioDefinition(ProjectDefinition& pd, const json& js) ;

        AudioDefinition(AudioDefinition&&) =default;
        AudioDefinition& operator=(AudioDefinition&&) =default;

        bool isFormatWav() const;
        bool isFormatOgg() const;

        void setLoop(bool);
        bool getLoop() const;
    };
}
