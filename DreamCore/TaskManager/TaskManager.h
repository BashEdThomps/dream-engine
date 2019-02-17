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

#include "../Common/DreamObject.h"
#include "TaskThread.h"
#include <memory>

using std::vector;
using std::shared_ptr;

namespace Dream
{
    class TaskManager : public DreamObject
    {
        vector<TaskThread*> mThreadVector;
        atomic<size_t> mNextThread;
    public:
         TaskManager();
         ~TaskManager();
         void startAllThreads();
         void joinAllThreads();
         void pushTask(Task* t);
         void pushDestructionTask(const shared_ptr<DestructionTask>& dt);
         void clearFences();
         void waitForFence();
         vector<TaskThread*> getThreadVector() const;
    };
}
