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
        PhysicsComponent* mComponent;
        PhysicsObjectRuntime* mRuntime;
    public:
        PhysicsAddObjectTask(ProjectRuntime* pr, PhysicsComponent* cp, PhysicsObjectRuntime* rt);
        void execute();
    };

    class PhysicsUpdateWorldTask : public Task
    {
        PhysicsComponent* mComponent;
    public:
        PhysicsUpdateWorldTask(ProjectRuntime* pr, PhysicsComponent* cp);
        void execute();
    };
}
