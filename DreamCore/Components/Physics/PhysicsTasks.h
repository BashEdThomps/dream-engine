#pragma once

#include "Task/Task.h"
#include "Components/Graphics/GraphicsComponentTasks.h"

namespace octronic::dream
{
    class PhysicsRuntime;
    class PhysicsComponent;

    // PhysicsAddObjectTask ===================================================

    class PhysicsAddObjectTask : public Task
    {
    public:
        PhysicsAddObjectTask(ProjectRuntime& pr, PhysicsRuntime& rt);
        void execute();
    protected:
        PhysicsRuntime& getPhysicsRuntime() const;
    private:
        reference_wrapper<PhysicsRuntime> mRuntime;
    };

    // PhysicsUpdateWorldTask ==================================================

    class PhysicsUpdateWorldTask : public Task
    {
    public:
        PhysicsUpdateWorldTask(ProjectRuntime& pr);
        void execute();
    };
}
