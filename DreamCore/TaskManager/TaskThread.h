#pragma once

#include <thread>
#include <vector>
#include <mutex>

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
        vector<Task*> mTaskQueue;
        mutex mTaskQueueMutex;
        atomic<bool> mRunning;
        atomic<bool> mFence;
        int mThreadId;
    public:

         TaskThread (int id);
         void join();
         void executeTaskQueue();
         void clearFence();
         bool getFence();
         bool pushTask(Task* t);
         void setRunning(volatile bool running);
         int getThreadId();
    };
}
