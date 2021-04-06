#pragma once

#include "Common/Uuid.h"
#include "Task/Task.h"

namespace octronic::dream
{
    class InputComponent;
    class ScriptRuntime;

    // InputPollDataTask =======================================================

    class InputPollDataTask : public Task
    {
    public:
        InputPollDataTask(ProjectRuntime& pr);
        void execute() override;
    };

    // InputRegisterScriptTask =================================================

    class InputRegisterScriptTask : public Task
    {
    public:
        InputRegisterScriptTask(ProjectRuntime& pr);
        void execute() override;
    };

    // InputExecuteScriptTask ==================================================

    class InputExecuteScriptTask : public Task
    {
    public:
        InputExecuteScriptTask(ProjectRuntime& pr);
        void execute() override;
    };

    // InputRemoveScriptTask ===================================================

    class InputRemoveScriptTask : public DestructionTask
    {
        UuidType mInputScriptUuid;
    public:
        InputRemoveScriptTask(ProjectRuntime& pr);
        void setInputScriptUuid(UuidType u);
        void execute() override;
    };
}
