#pragma once

#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <memory>

#include "../Common/DreamObject.h"
#include "Task.h"

using std::thread;
using std::mutex;
using std::vector;
using std::atomic;
using std::shared_ptr;

namespace Dream
{
    class TaskThread : public DreamObject
    {
    protected:
        thread mThread;
        vector<Task*> mDebugTaskQueue;
        vector<Task*> mTaskQueue;
        mutex mTaskQueueMutex;
        vector< shared_ptr<DestructionTask> > mDestructionTaskQueue;
        mutex mDestructionTaskQueueMutex;
        atomic<bool> mRunning;
        atomic<bool> mFence;
        atomic<int> mThreadId;
    public:

         TaskThread (int id);
         TaskThread(const TaskThread&& that);
         void join();
         void executeTaskQueue();
         void clearFence();
         bool getFence();
         bool pushTask(Task* t);
         bool pushDestructionTask(const shared_ptr<DestructionTask>& dt);
         void setRunning(bool running);
         int getThreadId();
         const vector<Task*>& getDebugTaskQueue();
    };
}
