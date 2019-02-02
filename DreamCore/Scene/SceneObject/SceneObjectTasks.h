#pragma once

#include "../../Components/TaskManager/Task.h"
#include "../../Components/Time.h"
#include "../../Project/ProjectRuntime.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"
#include "../../Scene/SceneRuntime.h"

namespace Dream
{
    class LifetimeUpdateTask : public Task
    {
    protected:
        SceneObjectRuntime* mSceneObjectRuntime;
    public:
        inline LifetimeUpdateTask(SceneObjectRuntime* rt)
            : Task(), mSceneObjectRuntime(rt)
        {
            #ifdef DREAM_LOG
            setLogClassName("LifetimeUpdateTask");
            #endif
            rt->setLifetimeUpdateTask(this);
        }

        inline  ~LifetimeUpdateTask() {}

        inline
        bool execute
        ()
        {
            mSceneObjectRuntime->lock();
            #ifdef DREAM_LOG
            getLog()->critical("Updating SO Lifetime for {}", mSceneObjectRuntime->getNameAndUuidString());
            #endif

            long timeDelta = mSceneObjectRuntime
                    ->getSceneRuntime()
                    ->getProjectRuntime()
                    ->getTime()
                    ->getFrameTimeDelta();

            long deferredFor = mSceneObjectRuntime->getDeferredFor();
            if (deferredFor > 0)
            {
                long deferral = deferredFor-timeDelta;
                #ifdef DREAM_LOG
                getLog()->critical("Reducing defferal by {} to {} for {}", timeDelta, deferral, mSceneObjectRuntime->getNameAndUuidString());
                #endif
                mSceneObjectRuntime->setDeferredFor(deferral);
                if (deferral < 0)
                {
                    #ifdef DREAM_LOG
                    getLog()->critical("Loading Deferred Runtime {}", mSceneObjectRuntime->getNameAndUuidString());
                    #endif
                    mSceneObjectRuntime->loadDeferred();
                }
            }
            else
            {
                mSceneObjectRuntime->increaseLifetime(timeDelta);
            }
            mSceneObjectRuntime->clearLifetimeUpdateTask();
            mSceneObjectRuntime->unlock();
            return true;
        }
    };
}
