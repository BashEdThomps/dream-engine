#pragma once

#include <thread>
#include <vector>
#include <mutex>

#include "Task.h"
#include "../../Common/DreamObject.h"

using std::thread;
using std::mutex;
using std::vector;

namespace Dream
{
    class TaskThread : public DreamObject
    {
    protected:
        thread *mThread;
        vector<Task*> mTaskQueue;
        mutex mTaskQueueMutex;
        volatile bool mRunning;
    public:

        TaskThread ()
            :DreamObject ("TaskThread"),
             mThread(nullptr)

        {
            mThread = new thread(&TaskThread::executeTaskQueue,this);
        }

        ~TaskThread()
        {
            if (mThread)
            {
                delete mThread;
            }
        }

        inline void join()
        {
            mThread->join();
        }

        inline void
        executeTaskQueue
        ()
        {
            mRunning = true;
            while (mRunning)
            {
                mTaskQueueMutex.lock();
                for (Task* t : mTaskQueue)
                {
                    if (!t->hasExpired())
                    {
                        t->execute();
                    }
                    delete t;
                }
                mTaskQueue.clear();
                mTaskQueueMutex.unlock();
                #ifdef DREAM_LOG
                getLog()->critical("Finished this lot of LogicUpdateTasks");
                #endif
                std::this_thread::yield();
            }
        }

        inline void pushTask(Task* t)
        {
            mTaskQueueMutex.lock();
            mTaskQueue.push_back(t);
            mTaskQueueMutex.unlock();
        }

        inline void setRunning(volatile bool running)
        {
            mRunning = running;
        }
    };
}
