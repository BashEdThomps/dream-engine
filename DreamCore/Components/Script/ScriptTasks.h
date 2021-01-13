#pragma once

#include "TaskManager/Task.h"
#include "Components/Graphics/GraphicsComponentTask.h"

namespace Dream
{
    class EntityRuntime;
    class ScriptRuntime;

    class ScriptConstructionTask : public Task
    {
        ScriptRuntime* mScript = nullptr;
    public:
        ScriptConstructionTask();
        void execute() override;
        void setScript(ScriptRuntime* rt);
    };

    class ScriptOnInitTask : public Task
    {
        EntityRuntime* mEntity;
        ScriptRuntime* mScript = nullptr;

    public:
        ScriptOnInitTask(EntityRuntime* rt);
        void execute() override;
        void setScript(ScriptRuntime* rt);
    };

    class ScriptOnUpdateTask : public Task
    {
        EntityRuntime* mEntity;
        ScriptRuntime* mScript = nullptr;

    public:
        ScriptOnUpdateTask(EntityRuntime* rt);
        void execute() override;
        void setScript(ScriptRuntime* rt);
    };

    class ScriptOnEventTask : public Task
    {
        EntityRuntime* mEntity;
        ScriptRuntime* mScript = nullptr;

    public:
        ScriptOnEventTask(EntityRuntime* rt);
        void execute() override;
        void setScript(ScriptRuntime* rt);
    };

    class ScriptOnDestroyTask : public DestructionTask
    {
        EntityRuntime* mParentEntity;
        ScriptRuntime* mScript = nullptr;
        uint32_t mDestroyedObject;
    public:
        ScriptOnDestroyTask(uint32_t destroyed, EntityRuntime* rt);
        void execute() override;
        void setScript(ScriptRuntime* rt);
    };
}
