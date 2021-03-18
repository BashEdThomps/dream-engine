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
        weak_ptr<EntityRuntime> mEntity;
    public:
        EntityScriptCreateStateTask(const weak_ptr<ProjectRuntime>& pr,
                                    const weak_ptr<EntityRuntime>& rt);
        void execute();
    };

    // =========================================================================

    class EntityScriptOnInitTask : public Task
    {
        weak_ptr<EntityRuntime> mEntity;
    public:
        EntityScriptOnInitTask(
            const weak_ptr<ProjectRuntime>& pr,
            const weak_ptr<EntityRuntime>& rt);
        void execute();
    };

    // =========================================================================

    class EntityScriptOnUpdateTask : public Task
    {
        weak_ptr<EntityRuntime> mEntity;
    public:
        EntityScriptOnUpdateTask(
            const weak_ptr<ProjectRuntime>& pr,
            const weak_ptr<EntityRuntime>& rt);
        void execute();
    };

    // =========================================================================

    class EntityScriptOnEventTask : public Task
    {
        weak_ptr<EntityRuntime> mEntity;
    public:
        EntityScriptOnEventTask(
            const weak_ptr<ProjectRuntime>& pr,
            const weak_ptr<EntityRuntime>& rt);
        void execute();
    };

    // =========================================================================

    class EntityScriptRemoveStateTask : public DestructionTask
    {
        UuidType mEntityUuid;
        weak_ptr<ScriptRuntime> mScript;
    public:
        EntityScriptRemoveStateTask(
            const weak_ptr<ProjectRuntime>& pr,
            UuidType entityUuid,
            const weak_ptr<ScriptRuntime>& sr);
        void execute();
    };
}
