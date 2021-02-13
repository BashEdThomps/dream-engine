#pragma once

#include "Common/Uuid.h"
#include "Task/Task.h"
#include "Components/Graphics/GraphicsComponentTasks.h"

namespace octronic::dream
{
    class EntityRuntime;
    class ScriptRuntime;

    // =========================================================================

    class EntityScriptCreateStateTask : public Task
    {
        EntityRuntime* mEntity;
        ScriptRuntime* mScript;
    public:
        EntityScriptCreateStateTask(ProjectRuntime* pr, EntityRuntime* rt);
        void execute();
        void setScript(ScriptRuntime* rt);
    };

    // =========================================================================

    class InputScriptCreateStateTask : public Task
    {
        ScriptRuntime* mScript;
    public:
        InputScriptCreateStateTask(ProjectRuntime* pr);
        void execute();
        void setScript(ScriptRuntime* rt);
    };

    // =========================================================================

    class EntityScriptOnInitTask : public Task
    {
        EntityRuntime* mEntity;
        ScriptRuntime* mScript;

    public:
        EntityScriptOnInitTask(ProjectRuntime* pr, EntityRuntime* rt);
        void execute();
        void setScript(ScriptRuntime* rt);
    };

    // =========================================================================

    class EntityScriptOnUpdateTask : public Task
    {
        EntityRuntime* mEntity;
        ScriptRuntime* mScript;

    public:
        EntityScriptOnUpdateTask(ProjectRuntime* pr, EntityRuntime* rt);
        void execute();
        void setScript(ScriptRuntime* rt);
    };

    // =========================================================================

    class EntityScriptOnEventTask : public Task
    {
        EntityRuntime* mEntity;
        ScriptRuntime* mScript;

    public:
        EntityScriptOnEventTask(ProjectRuntime* pr, EntityRuntime* rt);
        void execute();
        void setScript(ScriptRuntime* rt);
    };

    // =========================================================================

    class EntityScriptRemoveStateTask : public DestructionTask
    {
        UuidType mUuid;
        ScriptRuntime* mScript;
    public:
        EntityScriptRemoveStateTask(ProjectRuntime* pr, UuidType uuid);
        void execute();
        void setScript(ScriptRuntime* rt);
    };

    // =========================================================================

    class InputScriptRemoveStateTask : public DestructionTask
    {
        ScriptRuntime* mScript;
    public:
        InputScriptRemoveStateTask(ProjectRuntime* pr);
        void execute();
        void setScript(ScriptRuntime* rt);
    };

}
