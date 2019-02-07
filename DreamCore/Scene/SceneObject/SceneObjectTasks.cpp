#include "SceneObjectTasks.h"
#include "SceneObjectRuntime.h"
#include "../SceneRuntime.h"
#include "../../Project/ProjectRuntime.h"
#include "../../Components/Time.h"

namespace Dream
{
    LifetimeUpdateTask::LifetimeUpdateTask
    (SceneObjectRuntime* rt)
        : Task(), mSceneObjectRuntime(rt)
    {
        #ifdef DREAM_LOG
        setLogClassName("LifetimeUpdateTask");
        #endif
    }


    void
    LifetimeUpdateTask::execute
    ()
    {
        if (mSceneObjectRuntime->tryLock())
        {
            #ifdef DREAM_LOG
            getLog()->critical("{} Executing on thread {}",mSceneObjectRuntime->getNameAndUuidString(), mThreadId);
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
            mCompleted = true;
            mSceneObjectRuntime->unlock();
        }
        else
        {
            mDeferralCount++;
        }
    }
}
