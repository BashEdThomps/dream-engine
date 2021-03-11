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
    public:
        EntityScriptCreateStateTask(ProjectRuntime* pr, EntityRuntime* rt);
        void execute();
    };

    // =========================================================================

    class EntityScriptOnInitTask : public Task
    {
        EntityRuntime* mEntity;
    public:
        EntityScriptOnInitTask(ProjectRuntime* pr, EntityRuntime* rt);
        void execute();
    };

    // =========================================================================

    class EntityScriptOnUpdateTask : public Task
    {
        EntityRuntime* mEntity;
    public:
        EntityScriptOnUpdateTask(ProjectRuntime* pr, EntityRuntime* rt);
        void execute();
    };

    // =========================================================================

    class EntityScriptOnEventTask : public Task
    {
        EntityRuntime* mEntity;
    public:
        EntityScriptOnEventTask(ProjectRuntime* pr, EntityRuntime* rt);
        void execute();
    };

    // =========================================================================

    class EntityScriptRemoveStateTask : public DestructionTask
    {
        UuidType mEntityUuid;
        ScriptRuntime* mScript;
    public:
        EntityScriptRemoveStateTask(ProjectRuntime* pr,UuidType entityUuid,  ScriptRuntime* sr);
        void execute();
    };
}
