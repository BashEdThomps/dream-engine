#pragma once

#include "Task/Task.h"

namespace octronic::dream
{
    class InputComponent;
    class SceneRuntime;

    class InputPollDataTask : public Task
    {
        InputComponent* mComponent;
    public:
        InputPollDataTask(ProjectRuntime* pr, InputComponent* cp);
        void execute();
    };

    class InputExecuteScriptTask : public Task
    {
        InputComponent* mComponent;
    public:
        InputExecuteScriptTask(ProjectRuntime* pr, InputComponent* cp);
        void execute();
    };
}
