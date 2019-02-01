/*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "LogicComponent.h"
#include "LogicTasks.h"
#include "../../Components/Time.h"
#include "../../Scene/SceneRuntime.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"
#include "LogicTasks.h"

using std::thread;

namespace Dream
{
    LogicComponent::LogicComponent
    () : Component(),
        mTaskQueueThread(nullptr)
    {
        #ifdef DREAM_LOG
        setLogClassName("LogicComponent");
        startTaskQueueThread();
        #endif
    }

    LogicComponent::~LogicComponent
    ()
    {
        #ifdef DREAM_LOG
        getLog()->debug("Destroying Object");
        #endif
        joinTaskQueueThread();
    }

    bool
    LogicComponent::init
    ()
    {
        #ifdef DREAM_LOG
        getLog()->debug("Initialisation Done");
        #endif
        return true;
    }

    void
    LogicComponent::updateComponent
    (SceneRuntime* sr)
    {
        if (!mEnabled)
        {
            #ifdef DREAM_LOG
            auto log = getLog();
            log->warn("Update Disabled");
            #endif
            return;
        }

        beginUpdate();
        long timeDelta = mTime->getFrameTimeDelta();
        if (timeDelta <= Time::DELTA_MAX)
        {
            updateSceneRuntimeTime(sr);
            updateSceneObjects(sr);
        }
        endUpdate();
    }
    void
    LogicComponent::updateSceneObjects
    (SceneRuntime* sr)
    {
       for (auto* runt : mUpdateQueue)
       {
           mTaskQueueMutex.lock();
           mTaskQueue.push_back(new LogicUpdateTask(runt));
           mTaskQueueMutex.unlock();
       }
       clearUpdateQueue();
    }


    void
    LogicComponent::updateSceneRuntimeTime
    (SceneRuntime* sr)
    {
       long frameTime = mTime->getCurrentFrameTime();
       if (sr->getSceneStartTime() <= 0)
       {
           sr->setSceneStartTime(frameTime);
       }
       sr->setSceneCurrentTime(frameTime-sr->getSceneStartTime());
    }

    void
    LogicComponent::executeTaskQueue
    ()
    {
        while (mRunningTaskQueue)
        {
            mTaskQueueMutex.lock();
            for (LogicUpdateTask* t : mTaskQueue)
            {
                if (!t->hasExpired())
                {
                    t->execute();
                }
                delete t;
            }
            #ifdef DREAM_LOG
            getLog()->critical("Finished this lot of LogicUpdateTasks");
            #endif
            mTaskQueue.clear();
            mTaskQueueMutex.unlock();
            std::this_thread::yield();
        }
    }

    void
    LogicComponent::startTaskQueueThread
    ()
    {
        mRunningTaskQueue = true;
        mTaskQueueThread = new thread(&LogicComponent::executeTaskQueue,this);
    }

    void
    LogicComponent::joinTaskQueueThread
    ()
    {
        mRunningTaskQueue = false;
        mTaskQueueThread->join();
        delete mTaskQueueThread;
        mTaskQueueThread = nullptr;
    }
}
