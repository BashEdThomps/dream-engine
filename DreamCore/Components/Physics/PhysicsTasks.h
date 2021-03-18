#pragma once

#include "Task/Task.h"
#include "Components/Graphics/GraphicsComponentTasks.h"

namespace octronic::dream
{
    class PhysicsObjectRuntime;
    class PhysicsComponent;
    class PhysicsDebugDrawer;

    class PhysicsAddObjectTask : public Task
    {
        weak_ptr<PhysicsComponent> mComponent;
        weak_ptr<PhysicsObjectRuntime> mRuntime;
    public:
        PhysicsAddObjectTask(
                const weak_ptr<ProjectRuntime>& pr,
                const weak_ptr<PhysicsComponent>& cp,
                const weak_ptr<PhysicsObjectRuntime>& rt);
        void execute();
    };

    class PhysicsUpdateWorldTask : public Task
    {
        weak_ptr<PhysicsComponent> mComponent;
    public:
        PhysicsUpdateWorldTask(
                const weak_ptr<ProjectRuntime>& pr,
                const weak_ptr<PhysicsComponent>& cp);
        void execute();
    };
}
