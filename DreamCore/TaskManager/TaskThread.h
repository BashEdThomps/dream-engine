#pragma once

#include <thread>
#include <vector>
#include <mutex>

#include "Task.h"
#include "../Common/DreamObject.h"

using std::thread;
using std::mutex;
using std::vector;
using std::atomic;

namespace Dream
{
    class TaskThread : public DreamObject
    {
    protected:
        thread *mThread;
        vector<Task*> mTaskQueue;
        vector<Task*> mCompletedTasks;
        mutex mTaskQueueMutex;
        atomic<bool> mRunning;
        atomic<bool> mFence;
        unsigned int mThreadId;
    public:

        TaskThread (unsigned int id)
            : DreamObject ("TaskThread"),
              mThreadId(id),
              mThread(nullptr)
        {
            mThread = new thread(&TaskThread::executeTaskQueue,this);
        }

        ~TaskThread()
        {
            if (mThread)
            {
                delete mThread;
                mThread = nullptr;
            }
        }

        inline void join()
        {
            #ifdef DREAM_LOG
            getLog()->critical("Joining Thread {}",mThreadId);
            #endif
            mThread->join();
        }

        inline void
        executeTaskQueue
        ()
        {
            mRunning = true;
            while (mRunning)
            {
                if (mFence)
                {
                    std::this_thread::yield();
                    continue;
                }

                mTaskQueueMutex.lock();
                if (!mTaskQueue.empty())
                {
                    #ifdef DREAM_LOG
                    getLog()->critical("Worker {} has {} tasks",getThreadId(),mTaskQueue.size());
                    #endif
                    for (Task* t : mTaskQueue)
                    {
                        if (!t->hasExpired())
                        {
                            t->execute();
                            std::this_thread::yield();
                        }

                        if (!t->isDeferred())
                        {
                            mCompletedTasks.push_back(t);
                        }
                        else
                        {
                            #ifdef DREAM_LOG
                            getLog()->critical("Worker {} has deferred {} for {} time",getThreadId(),t->getClassName(),t->getDeferralCount());
                            #endif
                        }
                    }

                    // Remove completed tasks from queue
                    for(auto* t : mCompletedTasks)
                    {
                       auto itr = find(mTaskQueue.begin(),mTaskQueue.end(), t);
                       if (itr!=mTaskQueue.end())
                       {
                           mTaskQueue.erase(itr);
                           delete t;
                       }
                    }
                    mCompletedTasks.clear();

                    mFence = true;
                    mTaskQueueMutex.unlock();
                    #ifdef DREAM_LOG
                    getLog()->critical("Worker {} has finished running it's task queue, Setting Fence",getThreadId());
                    #endif
                }
                else
                {
                    mTaskQueueMutex.unlock();
                    std::this_thread::yield();
                }
            }
        }

        inline void clearFence()
        {
            #ifdef DREAM_LOG
            getLog()->critical("Clearing fence for thresd {}",getThreadId());
            #endif
            mFence = false;
        }

        inline bool getFence()
        {
            return mFence;
        }

        inline void pushTask(Task* t)
        {
            t->setThreadId(mThreadId);
            mTaskQueueMutex.lock();
            mTaskQueue.push_back(t);
            mTaskQueueMutex.unlock();
        }

        inline void setRunning(volatile bool running)
        {
            mRunning = running;
        }

        inline unsigned int getThreadId()
        {
            return mThreadId;
        }
    };
}
