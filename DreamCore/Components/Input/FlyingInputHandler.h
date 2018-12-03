#pragma once

#include "InputHandler.h"

namespace Dream
{
    class FlyingInputHandler : public InputHandler
    {
    public:
        FlyingInputHandler(SceneRuntime* sRunt);
        ~FlyingInputHandler() override;

        void onInput(InputComponent*) override;
    };
}
