#pragma once

#include "Components/Task/Task.h"
#include "Components/Graphics/GraphicsComponentTask.h"

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
        PhysicsAddObjectTask(PhysicsComponent* cp, PhysicsObjectRuntime* rt);
        void execute();
    };

    class PhysicsUpdateWorldTask : public Task
    {
        PhysicsComponent* mComponent;
    public:
        PhysicsUpdateWorldTask(PhysicsComponent* cp);
        void execute();
    };

    class PhysicsDrawDebugTask : public GraphicsComponentTask
    {
        PhysicsComponent* mComponent;
    public:
        PhysicsDrawDebugTask(PhysicsComponent* cp);
        void execute();
    };
}
