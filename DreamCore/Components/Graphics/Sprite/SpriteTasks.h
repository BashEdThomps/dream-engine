#pragma once

#include "Common/GLHeader.h"

// Defer GL Operations using Task objects
#include "Components/Graphics/GraphicsComponentTask.h"
#include "Common/Uuid.h"

namespace octronic::dream
{
    class SpriteRuntime;
    class TextureRuntime;

    class SpriteConstructionTask : public GraphicsComponentTask
    {
        SpriteRuntime* mSpriteRuntime;
    public:
        SpriteConstructionTask(SpriteRuntime* rt);
        void execute();
    };

    class SpriteDestructionTask : public GraphicsComponentDestructionTask
    {
        UuidType mEntityUuid;
        TextureRuntime* mTextureRuntime;

    public:
        SpriteDestructionTask(TextureRuntime* texture, UuidType entityUuid);
        void execute() override;
    };
}
