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
        InputPollDataTask(const shared_ptr<ProjectRuntime>& pr);
        void execute() override;
    };

    // InputRegisterScriptTask =================================================

    class InputRegisterScriptTask : public Task
    {
    public:
        InputRegisterScriptTask(const shared_ptr<ProjectRuntime>& pr);
        void execute() override;
    };

    // InputExecuteScriptTask ==================================================

    class InputExecuteScriptTask : public Task
    {
    public:
        InputExecuteScriptTask(const shared_ptr<ProjectRuntime>& pr);
        void execute() override;
    };

    // InputRemoveScriptTask ===================================================

    class InputRemoveScriptTask : public DestructionTask
    {
        UuidType mInputScriptUuid;
    public:
        InputRemoveScriptTask(const shared_ptr<ProjectRuntime>& pr);
        void setInputScriptUuid(UuidType u);
        void execute() override;
    };
}
