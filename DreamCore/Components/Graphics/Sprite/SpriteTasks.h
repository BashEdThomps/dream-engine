#pragma once

#include "Common/GLHeader.h"

// Defer GL Operations using Task objects
#include "Components/Graphics/GraphicsComponentTask.h"

namespace octronic::dream
{
    class SpriteRuntime;

    class SpriteConstructionTask : public GraphicsComponentTask
    {
        SpriteRuntime* mSpriteRuntime;
    public:
        SpriteConstructionTask(SpriteRuntime* rt);
        void execute();
    };

    class SpriteDestructionTask : public GraphicsComponentDestructionTask
    {
    public:
        SpriteDestructionTask();
        void execute() override;
    };
}
