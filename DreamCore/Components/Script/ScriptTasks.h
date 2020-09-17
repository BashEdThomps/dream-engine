#pragma once

#include "../../TaskManager/Task.h"
#include "../Graphics/GraphicsComponentTask.h"

namespace Dream
{
    class ActorRuntime;
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
        ActorRuntime* mActor;
        ScriptRuntime* mScript = nullptr;

    public:
        ScriptOnInitTask(ActorRuntime* rt);
        void execute() override;
        void setScript(ScriptRuntime* rt);
    };

    class ScriptOnUpdateTask : public Task
    {
        ActorRuntime* mActor;
        ScriptRuntime* mScript = nullptr;

    public:
        ScriptOnUpdateTask(ActorRuntime* rt);
        void execute() override;
        void setScript(ScriptRuntime* rt);
    };

    class ScriptOnEventTask : public Task
    {
        ActorRuntime* mActor;
        ScriptRuntime* mScript = nullptr;

    public:
        ScriptOnEventTask(ActorRuntime* rt);
        void execute() override;
        void setScript(ScriptRuntime* rt);
    };

    class ScriptOnDestroyTask : public DestructionTask
    {
        ActorRuntime* mParentActor;
        ScriptRuntime* mScript = nullptr;
        uint32_t mDestroyedObject;
    public:
        ScriptOnDestroyTask(uint32_t destroyed, ActorRuntime* rt);
        void execute() override;
        void setScript(ScriptRuntime* rt);
    };
}
