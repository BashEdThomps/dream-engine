#pragma once

#include "Common/Uuid.h"
#include "Components/Task/Task.h"
#include "Components/Graphics/GraphicsComponentTask.h"

namespace octronic::dream
{
    class EntityRuntime;
    class ScriptRuntime;

    // =========================================================================

    class EntityScriptConstructionTask : public Task
    {
        EntityRuntime* mEntity;
        ScriptRuntime* mScript;
    public:
        EntityScriptConstructionTask(EntityRuntime* rt);
        void execute();
        void setScript(ScriptRuntime* rt);
    };

    // =========================================================================

    class InputScriptConstructionTask : public Task
    {
        ScriptRuntime* mScript;
    public:
        InputScriptConstructionTask();
        void execute();
        void setScript(ScriptRuntime* rt);
    };

    // =========================================================================

    class EntityScriptOnInitTask : public Task
    {
        EntityRuntime* mEntity;
        ScriptRuntime* mScript;

    public:
        EntityScriptOnInitTask(EntityRuntime* rt);
        void execute();
        void setScript(ScriptRuntime* rt);
    };

    // =========================================================================

    class EntityScriptOnUpdateTask : public Task
    {
        EntityRuntime* mEntity;
        ScriptRuntime* mScript;

    public:
        EntityScriptOnUpdateTask(EntityRuntime* rt);
        void execute();
        void setScript(ScriptRuntime* rt);
    };

    // =========================================================================

    class EntityScriptOnEventTask : public Task
    {
        EntityRuntime* mEntity;
        ScriptRuntime* mScript;

    public:
        EntityScriptOnEventTask(EntityRuntime* rt);
        void execute();
        void setScript(ScriptRuntime* rt);
    };

    // =========================================================================

    class EntityScriptDestructionTask : public DestructionTask
    {
        UuidType mUuid;
        ScriptRuntime* mScript;
    public:
        EntityScriptDestructionTask(UuidType uuid);
        void execute();
        void setScript(ScriptRuntime* rt);
    };

    // =========================================================================

    class InputScriptDestructionTask : public DestructionTask
    {
        ScriptRuntime* mScript;
    public:
        InputScriptDestructionTask();
        void execute();
        void setScript(ScriptRuntime* rt);
    };

}
