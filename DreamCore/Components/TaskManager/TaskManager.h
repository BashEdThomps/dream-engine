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

#pragma once

#include "../../Common/DreamObject.h"
#include "TaskThread.h"

namespace Dream
{

    class TaskManager : public DreamObject
    {
        vector<TaskThread*> mThreadVector;
        size_t mNextThread;
    public:

        inline TaskManager()
            : DreamObject("TaskManager"),
              mNextThread(0)
        {
            #ifdef DREAM_LOG
            startAllThreads();
            #endif
        }

        inline ~TaskManager() override
        {
            #ifdef DREAM_LOG
            getLog()->debug("Destroying Object");
            #endif
            joinAllThreads();
        }

        inline void startAllThreads()
        {
            for (unsigned int i=0; i < thread::hardware_concurrency(); i++)
            {
                mThreadVector.push_back(new TaskThread());
            }
        }

        inline void joinAllThreads()
        {
            for (auto* t : mThreadVector)
            {
                t->setRunning(false);
            }
            for (auto* t : mThreadVector)
            {
                t->join();
                delete t;
            }
            mThreadVector.clear();
        }

        inline void pushTask(Task* t)
        {
            mThreadVector.at(mNextThread)->pushTask(t);
            mNextThread = (mNextThread +1) % mThreadVector.size();
        }
    };
}
