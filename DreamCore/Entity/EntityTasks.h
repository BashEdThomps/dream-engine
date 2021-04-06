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
        reference_wrapper<EntityRuntime> mEntity;
    public:
        EntityScriptCreateStateTask(ProjectRuntime& pr, EntityRuntime& rt);
        void execute();
    };

    // =========================================================================

    class EntityScriptOnInitTask : public Task
    {
        reference_wrapper<EntityRuntime> mEntity;
    public:
        EntityScriptOnInitTask(ProjectRuntime& pr, EntityRuntime& rt);
        void execute();
    };

    // =========================================================================

    class EntityScriptOnUpdateTask : public Task
    {
        reference_wrapper<EntityRuntime> mEntity;
    public:
        EntityScriptOnUpdateTask(ProjectRuntime& pr, EntityRuntime& rt);
        void execute();
    };

    // =========================================================================

    class EntityScriptOnEventTask : public Task
    {
        reference_wrapper<EntityRuntime> mEntity;
    public:
        EntityScriptOnEventTask(ProjectRuntime& pr, EntityRuntime& rt);
        void execute();
    };

    // =========================================================================

    class EntityScriptRemoveStateTask : public DestructionTask
    {
        UuidType mEntityUuid;
        reference_wrapper<ScriptRuntime> mScript;
    public:
        EntityScriptRemoveStateTask(ProjectRuntime& pr,UuidType entityUuid, ScriptRuntime& sr);
        void execute();
    };
}
