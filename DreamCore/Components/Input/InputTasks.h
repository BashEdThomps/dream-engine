#pragma once

#include "../../TaskManager/Task.h"

namespace Dream
{
    class InputComponent;
    class SceneRuntime;

    class InputPollDataTask : public Task
    {
        InputComponent* mComponent;
    public:
        InputPollDataTask(InputComponent* cp);
        void execute();
    };

    class InputExecuteScriptTask : public Task
    {
        InputComponent* mComponent;
        SceneRuntime* mSceneRuntime;
    public:
        InputExecuteScriptTask(InputComponent* cp, SceneRuntime* rt);
        void execute();
    };
}
