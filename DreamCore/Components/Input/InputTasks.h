#pragma once

#include "Components/Task/Task.h"

namespace octronic::dream
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
