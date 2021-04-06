#pragma once

#include "Task/Task.h"
#include "Components/Graphics/GraphicsComponentTasks.h"

namespace octronic::dream
{
    class PhysicsObjectRuntime;
    class PhysicsComponent;

    // PhysicsAddObjectTask ===================================================

    class PhysicsAddObjectTask : public Task
    {
    public:
        PhysicsAddObjectTask(ProjectRuntime& pr, PhysicsObjectRuntime& rt);
        void execute();
    protected:
        PhysicsObjectRuntime& getPoRuntime() const;
    private:
        reference_wrapper<PhysicsObjectRuntime> mRuntime;
    };

    // PhysicsUpdateWorldTask ==================================================

    class PhysicsUpdateWorldTask : public Task
    {
    public:
        PhysicsUpdateWorldTask(ProjectRuntime& pr);
        void execute();
    };
}
