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

#include <mutex>

namespace octronic::dream
{
    TaskManager::TaskManager()
        :
          mNextThread(0),
          mHardwareThreadCount(thread::hardware_concurrency())
    {
        LOG_CRITICAL("TaskManager: Constructing with {} hardware threads available", mHardwareThreadCount);
        startAllThreads();
    }

    TaskManager::~TaskManager()
    {
        LOG_TRACE("TaskManager: Destroying Object");
        joinAllThreads();
    }

    void TaskManager::startAllThreads()
    {
        LOG_DEBUG("TaskManager: Creating {} threads...",mHardwareThreadCount);

        for (unsigned int i=0; i <  mHardwareThreadCount; i++)
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

    void TaskManager::pushTask(Task* task)
    {
        assert(task != nullptr);

        for (TaskThread* thread : mThreadVector)
        {
            if (thread->hasTask(task))
            {
                LOG_INFO("TaskManager: Thread {} already has task {}",thread->getThreadID(), task->getTaskName());
                return;
            }
        }
        while (true)
        {
            bool result = mThreadVector.at(mNextThread)->pushTask(task);
            if (result)
            {
                LOG_DEBUG("TaskManager: {} pushed task to thread {}",task->getTaskName(), mNextThread);
            }
            else
            {
                LOG_ERROR("TaskManager: Failed to push task {} to thread {}",task->getTaskName(), mNextThread);
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


    void TaskManager::allowThreadsToRun()
    {

        for (TaskThread* t : mThreadVector)
        {
            t->clearWaitingToRunFence();
        }
    }

    void TaskManager::waitForThreadsToFinish()
    {
        LOG_TRACE("TaskManager: ... Waiting for Threads to Finish ...");
        int tries = 0;
        while (true)
        {
            tries++;
            //assert(tries < 1000);
            LOG_TRACE("TaskManager: Waiting for Threads to Finish for {} Time",tries);
            bool result = true;
            for (TaskThread* task_thread : mThreadVector)
            {
                result = (result && task_thread->getWaitingToRunFence());
                if (!result)
                {
                    LOG_TRACE("TaskManager: Thread {} is still working on {} tasks", task_thread->getThreadID(), task_thread->getNumTasks());
                    break;
                }
            }
            if (result)
            {
                LOG_TRACE("TaskManager: All Threads have Finished");
                return;
            }
            std::this_thread::yield();
        }
    }

    vector<TaskThread*>& TaskManager::getThreadVector()
    {
        return mThreadVector;
    }
}
