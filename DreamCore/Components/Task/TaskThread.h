#pragma once

#include "Task.h"
#include "Common/LockableObject.h"

#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <memory>

using std::thread;
using std::mutex;
using std::vector;
using std::atomic;
using std::shared_ptr;

namespace octronic::dream
{
    /**
     * @brief The TaskThread class is used by the TaskManager to run Tasks. Each
     * TaskThread encapsulates a std::Thread running a while loop, which executes
     * incoming Tasks.
     */
    class TaskThread : public LockableObject
    {

    public:
         TaskThread (int id);
         void join();
         void executeTaskQueue();
         void clearWaitingToRunFence();
         bool getWaitingToRunFence();
         bool pushTask(Task* t);
         bool pushDestructionTask(const shared_ptr<DestructionTask>& dt);
         void setRunning(bool running);
         int getThreadID();
         bool hasTask(Task* t);
         size_t getNumTasks();

    private:
        thread mThread;
        vector<Task*> mTaskQueue;
        vector< shared_ptr<DestructionTask> > mDestructionTaskQueue;
        atomic<bool> mRunning;
        atomic<bool> mWaitingToRunFence;
        atomic<int> mThreadID;
    };
}
