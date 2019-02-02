#pragma once

#include "../TaskManager/Task.h"
#include "../Graphics/GraphicsComponentTask.h"
#include "PhysicsObjectRuntime.h"
#include "PhysicsComponent.h"
#include "PhysicsDebugDrawer.h"

namespace Dream
{
    class PhysicsAddObjectTask : public Task
    {
        PhysicsObjectRuntime* mRuntime;
        PhysicsComponent* mComponent;
    public:
        inline PhysicsAddObjectTask(PhysicsComponent* cp, PhysicsObjectRuntime* rt)
            : Task(),
              mComponent(cp),
              mRuntime(rt)
        {

            #ifdef DREAM_LOG
            setLogClassName("PhysicsAddObjectTask");
            #endif
            mRuntime->setAddObjectTask(this);
        }

        inline bool execute() override
        {
            #ifdef DREAM_LOG
            getLog()->trace("Adding SceneObject to physics world");
            #endif
            mRuntime->lock();
            mComponent->lock();
            mComponent->addPhysicsObjectRuntime(mRuntime);
            mComponent->unlock();
            mRuntime->setInPhysicsWorld(true);
            mRuntime->setAddObjectTask(nullptr);
            mRuntime->unlock();
            return true;
        }
    };

    class PhysicsUpdateWorldTask : public Task
    {
        PhysicsComponent* mComponent;
    public:
        PhysicsUpdateWorldTask(PhysicsComponent* cp)
            : Task(), mComponent(cp)
        {
            #ifdef DREAM_LOG
            setLogClassName("PhysicsComponentUpdateTask");
            #endif
            mComponent->setUpdateWorldTask(this);
        }

        inline bool execute() override
        {
            mComponent->lock();
            mComponent->stepSimulation();
            mComponent->setUpdateWorldTask(nullptr);
            mComponent->unlock();
            return true;
        }
    };

    class PhysicsDrawDebugTask : public GraphicsComponentTask
    {
        PhysicsComponent* mComponent;
    public:
        inline PhysicsDrawDebugTask
        (PhysicsComponent* cp)
            : GraphicsComponentTask(), mComponent(cp)
        {
            #ifdef DREAM_LOG
            setLogClassName("PhysicsDrawDebugTask");
            #endif
            mComponent->setDrawDebugTask(this);
        }

        inline bool execute() override
        {
            mComponent->lock();
            mComponent->getDebugDrawer()->drawAll();
            mComponent->setDrawDebugTask(nullptr);
            mComponent->unlock();
            return true;
        }
    };
}
