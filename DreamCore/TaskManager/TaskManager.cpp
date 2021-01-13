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

namespace Dream
{
    vector<TaskThread*> TaskManager::getThreadVector() const
    {
        return mThreadVector;
    }

    TaskManager::TaskManager()
        : mNextThread(0)
        {
            startAllThreads();
        }

        TaskManager::~TaskManager()
        {
            LOG_DEBUG("Destroying Object");
            joinAllThreads();
        }

        void TaskManager::startAllThreads()
        {
            LOG_CRITICAL("Starting all worker threads...");

            for (int i=0; i <  static_cast<int>(thread::hardware_concurrency()); i++)
            {
                LOG_CRITICAL("Spawning thread {}",i);
                mThreadVector.push_back(new TaskThread(i));
            }
        }

        void TaskManager::joinAllThreads()
        {
            LOG_CRITICAL("Joining all threads...");
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
            while (true)
            {
                bool result = mThreadVector.at(mNextThread)->pushTask(t);
                mNextThread = (mNextThread +1) % mThreadVector.size();
                if (result)
                {
                    LOG_CRITICAL("Task: pushed task to worker {}",mNextThread);
                    break;
                }
                std::this_thread::yield();
            }
        }

        void TaskManager::pushDestructionTask(const shared_ptr<DestructionTask>& dt)
        {
            while (true)
            {
                bool result = mThreadVector.at(mNextThread)->pushDestructionTask(dt);
                mNextThread = (mNextThread +1) % mThreadVector.size();
                if (result)
                {
                    LOG_CRITICAL("Task: pushed to worker {}",mNextThread);
                    break;
                }
                std::this_thread::yield();
            }
        }


        void TaskManager::clearFences()
        {
            LOG_CRITICAL("Clearing all fences");
            for (TaskThread* t : mThreadVector)
            {
               t->clearFence();
            }
        }

        void TaskManager::waitForFence()
        {
           LOG_CRITICAL("... Waiting for fence ...");
           int trys = 0;
           while (true)
           {
               bool result = true;
               for (TaskThread* t : mThreadVector)
               {
                   trys++;
                   LOG_TRACE("Trying for {} time",trys);

                   result = result && t->getFence();
                   if (!result)
                   {
                       LOG_TRACE("Thread {} is still working",t->getThreadId());
                       break;
                   }
               }
               if (result)
               {
                   LOG_CRITICAL("All Fences hit");
                   break;
               }
               std::this_thread::yield();
           }
        }
}
