#pragma once

#include <thread>
#include <vector>
#include <mutex>
#include <atomic>

#include "../Common/DreamObject.h"

using std::thread;
using std::mutex;
using std::vector;
using std::atomic;

namespace Dream
{
    class Task;
    class TaskThread : public DreamObject
    {
    protected:
        thread mThread;
        vector<Task*> mDebugTaskQueue;
        vector<Task*> mTaskQueue;
        mutex mTaskQueueMutex;
        atomic<bool> mRunning;
        atomic<bool> mFence;
        int mThreadId;
    public:

         TaskThread (int id);
         TaskThread(const TaskThread&& that);
         void join();
         void executeTaskQueue();
         void clearFence();
         bool getFence();
         bool pushTask(Task* t);
         void setRunning(volatile bool running);
         int getThreadId();
         const vector<Task*>& getDebugTaskQueue();
    };
}
