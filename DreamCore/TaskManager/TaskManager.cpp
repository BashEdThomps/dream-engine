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

namespace Dream
{
    vector<TaskThread*> TaskManager::getThreadVector() const
    {
        return mThreadVector;
    }

    TaskManager::TaskManager()
        : DreamObject("TaskManager"),
          mNextThread(0)
        {
            startAllThreads();
        }

        TaskManager::~TaskManager()
        {
            #ifdef DREAM_LOG
            getLog()->debug("Destroying Object");
            #endif
            joinAllThreads();
        }

        void TaskManager::startAllThreads()
        {
            #ifdef DREAM_LOG
            getLog()->critical("Starting all worker threads...");
            #endif

            for (int i=0; i <  static_cast<int>(thread::hardware_concurrency()); i++)
            {
                #ifdef DREAM_LOG
                getLog()->critical("Spawning thread {}",i);
                #endif
                mThreadVector.emplace_back(new TaskThread(i));
            }
        }

        void TaskManager::joinAllThreads()
        {
            #ifdef DREAM_LOG
            getLog()->critical("Joining all threads...");
            #endif
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
                    #ifdef DREAM_LOG
                    getLog()->critical("{} pushed to worker {}",t->getClassName(),mNextThread);
                    #endif
                    break;
                }
                std::this_thread::yield();
            }
        }

        void TaskManager::clearFences()
        {
            #ifdef DREAM_LOG
            getLog()->critical("Clearing all fences");
            #endif
            for (TaskThread* t : mThreadVector)
            {
               t->clearFence();
            }
        }

        void TaskManager::waitForFence()
        {
           #ifdef DREAM_LOG
           getLog()->critical("... Waiting for fence ...");
           #endif
           while (true)
           {
               bool result = true;
               for (TaskThread* t : mThreadVector)
               {
                   result = result && t->getFence();
               }
               if (result)
               {
                   #ifdef DREAM_LOG
                   getLog()->critical("All Fences hit");
                   #endif
                   break;
               }
               std::this_thread::yield();
           }
        }
}
