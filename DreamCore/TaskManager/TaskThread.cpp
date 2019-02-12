#include "TaskThread.h"
#include "Task.h"

#include <algorithm>
#include <angelscript.h>

using std::find;

namespace Dream
{
    const vector<Task*>& TaskThread::getDebugTaskQueue()
    {
        sort(mDebugTaskQueue.begin(),mDebugTaskQueue.end(),
             [&](Task* t1, Task* t2){return t1->getTaskId() > t2->getTaskId();});
        return mDebugTaskQueue;
    }

    TaskThread::TaskThread (int id)
        : DreamObject ("TaskThread"),
          mThread(thread(&TaskThread::executeTaskQueue,this)),
              mRunning(false),
              mFence(false),
              mThreadId(id)
        {
        }

        void TaskThread::join()
        {
            #ifdef DREAM_LOG
            getLog()->critical("Joining Thread {}",mThreadId);
            #endif
            mThread.join();
        }

        void TaskThread::executeTaskQueue()
        {
            vector<Task*> completed;
            mRunning = true;
            while (mRunning)
            {
                if (mFence)
                {
                    std::this_thread::yield();
                    continue;
                }

                mDebugTaskQueue.clear();
                if(mTaskQueueMutex.try_lock())
                {
                    while (!mTaskQueue.empty())
                    {
                        #ifdef DREAM_LOG
                        getLog()->critical("Worker {} has {} tasks",getThreadId(),mTaskQueue.size());
                        #endif
                        for (auto itr = mTaskQueue.begin(); itr != mTaskQueue.end(); itr++)
                        {
                            if (find(mDebugTaskQueue.begin(),mDebugTaskQueue.end(),(*itr)) == mDebugTaskQueue.end())
                            {
                                mDebugTaskQueue.push_back((*itr));
                            }
                            auto* t = (*itr);
                            // Check if ready to execute
                            if (t->isWaitingForDependencies())
                            {
                                t->incrementDeferralCount();
                            }
                            else
                            {
                                t->setState(TaskState::ACTIVE);
                                t->execute();
                            }

                            if (t->getState() == TaskState::COMPLETED)
                            {
                               completed.push_back(t);
                            }
                        }

                        for (auto* t : completed)
                        {
                            t->notifyDependents();
                            auto itr = find(mTaskQueue.begin(),mTaskQueue.end(), t);
                            if (itr != mTaskQueue.end())
                            {
                                mTaskQueue.erase(itr);
                            }
                        }
                        completed.clear();
                    }
                    mTaskQueueMutex.unlock();
                }

                #ifdef DREAM_LOG
                getLog()->critical("Worker {} has finished running it's task queue, Setting Fence",getThreadId());
                #endif
                mFence = true;
                std::this_thread::yield();
            }
            #ifdef DREAM_LOG
            getLog()->critical("---------- Worker {} is ending it's task queue",getThreadId());
            #endif
            int r = asThreadCleanup(); assert(r>=0);
        }

        void TaskThread::clearFence()
        {
            #ifdef DREAM_LOG
            getLog()->critical("Clearing fence for thread {}",getThreadId());
            #endif
            mFence = false;
        }

        bool TaskThread::getFence()
        {
            return mFence;
        }

        bool TaskThread::pushTask(Task* t)
        {
            if(mTaskQueueMutex.try_lock())
            {
                mTaskQueue.push_back(t);
                t->setThreadId(mThreadId);
                mTaskQueueMutex.unlock();
                return true;
            }
            return false;
        }

        void TaskThread::setRunning(volatile bool running)
        {
            mRunning = running;
        }

        int TaskThread::getThreadId()
        {
            return mThreadId;
        }
}
