#pragma once

#include "TaskManager/Task.h"

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
    public:
        InputExecuteScriptTask(InputComponent* cp);
        void execute();
    };
}
