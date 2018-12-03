#pragma once

#include "../../Common/DreamObject.h"

namespace Dream
{
    class SceneRuntime;
    class InputComponent;

    class InputHandler : public DreamObject
    {
    public:
        InputHandler(SceneRuntime*);
        ~InputHandler() override;
        virtual void onInput(InputComponent*) = 0;
    protected:
        SceneRuntime* mSceneRuntime;
    };
}
