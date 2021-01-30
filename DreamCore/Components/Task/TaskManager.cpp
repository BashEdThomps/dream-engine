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


#include "Task.h"
#include "TaskManager.h"
#include "TaskThread.h"
#include "Common/Logger.h"

namespace octronic::dream
{
    TaskManager::TaskManager()
        : mNextThread(0),
          mHardwareThreadCount(thread::hardware_concurrency()-1)
    {
        LOG_CRITICAL("TaskManager: Constructing with {} hardware threads available",
                     mHardwareThreadCount);
        startAllThreads();
    }

    TaskManager::~TaskManager()
    {
        LOG_TRACE("TaskManager: Destroying Object");
        joinAllThreads();
    }

    void TaskManager::startAllThreads()
    {
        LOG_DEBUG("TaskManager: Starting all threads...");

        for (int i=0; i <  mHardwareThreadCount; i++)
        {
            LOG_DEBUG("TaskManager: Spawning thread {}",i);
            mThreadVector.push_back(new TaskThread(i));
        }
    }

    void TaskManager::joinAllThreads()
    {
        LOG_DEBUG("TaskManager: Joining all threads...");
        for (TaskThread* t : mThreadVector)
        {
            t->setRunning(false);
        }
        for (TaskThread* t : mThreadVector)
        {
            t->join();
            delete t;
        }
        mThreadVector.clear();
    }

    void TaskManager::pushTask(Task* t)
    {
        for (TaskThread* thread : mThreadVector)
        {
            if (thread->hasTask(t))
            {
                LOG_ERROR("TaskManager: Thread {} already has task {}({})",
                          thread->getThreadId(), t->getTaskName(), t->getTaskId());
                return;
            }
        }
        while (true)
        {
            bool result = mThreadVector.at(mNextThread)->pushTask(t);
            if (result)
            {
                LOG_DEBUG("TaskManager: {}({}) pushed task to thread {}",t->getTaskName(),t->getTaskId(), mNextThread);
            }
            else
            {
                LOG_ERROR("TaskManager: Failed to push task {}({}) to thread {}",t->getTaskName(), t->getTaskId(), mNextThread);
            }
            mNextThread++;
            mNextThread = mNextThread % mThreadVector.size();
            assert(mNextThread >= 0 && mNextThread <= mThreadVector.size());
            if (result) break;
            std::this_thread::yield();
        }
    }

    void TaskManager::pushDestructionTask(const shared_ptr<DestructionTask>& dt)
    {
        while (true)
        {
            bool result = mThreadVector.at(mNextThread)->pushDestructionTask(dt);
            if (result)
            {
                LOG_DEBUG("TaskManager: {} pushed destruction task to thread {}",dt->getTaskName(), mNextThread);
            }
            else
            {
                LOG_ERROR("TaskManager: Failed to push task {} to thread {}",dt->getTaskName(), mNextThread);
            }
            mNextThread++;
            mNextThread = mNextThread % mThreadVector.size();
            if (result) break;
            std::this_thread::yield();
        }
    }


    void TaskManager::clearFences()
    {
        LOG_TRACE("TaskManager: Clearing all fences");
        for (TaskThread* t : mThreadVector)
        {
            t->clearFence();
        }
    }

    void TaskManager::waitForFence()
    {
        LOG_TRACE("TaskManager: ... Waiting for fence ...");
        int trys = 0;
        while (true)
        {
            bool result = true;
            for (TaskThread* t : mThreadVector)
            {
                trys++;
                LOG_TRACE("TaskManager: Waiting for fence for {} time",trys);

                result = (result && t->getFence());
                if (!result)
                {
                    LOG_TRACE("TaskManager: Thread {} is still working", t->getThreadId());
                    break;
                }
            }
            if (result)
            {
                LOG_TRACE("TaskManager: All Fences hit");
                break;
            }
            std::this_thread::yield();
        }
    }

    vector<TaskThread*> TaskManager::getThreadVector() const
    {
        return mThreadVector;
    }
}
