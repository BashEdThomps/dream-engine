#pragma once

#include "../Task.h"
#include "../Time.h"
#include "Animation/AnimationRuntime.h"
#include "Path/PathRuntime.h"
#include "Scroller/ScrollerRuntime.h"
#include "../../Project/ProjectRuntime.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"
#include "../../Scene/SceneRuntime.h"

namespace Dream
{
   class LogicUpdateTask : public Task
   {
   protected:
       SceneObjectRuntime* mSceneObjectRuntime;
   public:
    inline LogicUpdateTask(SceneObjectRuntime* rt)
        : Task(), mSceneObjectRuntime(rt)
    {
        setLogClassName("LogicUpdateTask");
        rt->setLogicUpdateTask(this);
    }

    inline  ~LogicUpdateTask() {}

    inline bool execute()
    {
         mSceneObjectRuntime->lock();
         if(updateSceneObjectLifetime())
         {
           updateAnimation();
           updatePath();
           updateScroller();
         }
         mSceneObjectRuntime->clearLogicUpdateTask();
         mSceneObjectRuntime->unlock();
         return true;
    }

    inline void updateAnimation()
    {
        if (mSceneObjectRuntime->hasAnimationRuntime())
        {
            #ifdef DREAM_LOG
            getLog()->critical("Updating Animation for {}", mSceneObjectRuntime->getNameAndUuidString());
            #endif
            mSceneObjectRuntime->getAnimationRuntime()->update();
        }
    }

    inline void updateScroller()
    {
        if (mSceneObjectRuntime->hasScrollerRuntime())
        {
            #ifdef DREAM_LOG
            getLog()->critical("Updating Scroller for {}", mSceneObjectRuntime->getNameAndUuidString());
            #endif
            mSceneObjectRuntime->getScrollerRuntime()->update();
        }
    }

    inline void updatePath()
    {
        if (mSceneObjectRuntime->hasPathRuntime())
        {
            #ifdef DREAM_LOG
            getLog()->critical("Updating Path for {}", mSceneObjectRuntime->getNameAndUuidString());
            #endif
            mSceneObjectRuntime->getPathRuntime()->update();
        }
    }

    inline bool updateSceneObjectLifetime()
    {
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
            return true;
        }
        return false;
    }
   };
}
