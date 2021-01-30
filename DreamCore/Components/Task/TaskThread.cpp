#include "TaskThread.h"
#include "Task.h"
#include "Common/Logger.h"

#include <algorithm>

using std::find;

namespace octronic::dream
{

    TaskThread::TaskThread (int id)
        : mThread(thread(&TaskThread::executeTaskQueue,this)),
          mRunning(false),
          mFence(true),
          mThreadId(id)
    {
    }

    void TaskThread::join()
    {
        LOG_TRACE("TaskThread: Joining Thread {}",mThreadId);
        mThread.join();
    }

    void TaskThread::executeTaskQueue()
    {
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
        		vector<Task*> completed;
                while (!mTaskQueue.empty())
                {
                    LOG_TRACE("TaskThread: Worker {} has {} tasks",getThreadId(),mTaskQueue.size());
                    for (auto itr = mTaskQueue.begin(); itr != mTaskQueue.end(); itr++)
                    {
                        if (find(mDebugTaskQueue.begin(),mDebugTaskQueue.end(),
                                 (*itr)) == mDebugTaskQueue.end())
                        {
                            mDebugTaskQueue.push_back((*itr));
                        }

                        Task* t = (*itr);
                        // Check if ready to execute

                        // Task has expired and does not need to execute
                        if (t->getState() == TaskState::TASK_STATE_EXPIRED)
                        {
                            LOG_TRACE("TaskThread: Task {}({}) has expired on thread {}, welp",
                                      t->getTaskName(), t->getTaskId(), mThreadId);
                            t->setState(TaskState::TASK_STATE_COMPLETED);
                        }
                        // Task failed
                        else if (t->getState() == TaskState::TASK_STATE_FAILED)
                        {
                            LOG_TRACE("TaskThread: Task {}({}) FAILED on thread {}, welp",
                                      t->getTaskName(), t->getTaskId(), mThreadId);
                           assert(false);
                        }
                        // Task is in good state, is it waiting for dependencies?
                        // if so, defer
                        else if (t->isWaitingForDependencies())
                        {
                            LOG_TRACE("TaskThread: Task {}({}) is waiting for dependencies on thread {}",
                                      t->getTaskName(), t->getTaskId(), mThreadId);
                            t->incrementDeferralCount();
                        }
                        // Not waiting for dependencies, so mark active and
                        // execute
                        else
                        {
                            LOG_TRACE("TaskThread: Task {}({}) is ready on thread {}",
                                      t->getTaskName(), t->getTaskId(), mThreadId);
                            t->setState(TaskState::TASK_STATE_ACTIVE);
                            t->execute();
                        }

                        // Task was completed, i.e. not deferred and not
                        // failed
                        if (t->getState() == TaskState::TASK_STATE_COMPLETED)
                        {
                            LOG_TRACE("TaskThread: Task {}({}) was completed on thread {}",
                                      t->getTaskName(), t->getTaskId(), mThreadId);
                            completed.push_back(t);
                        }
                    }

                    // Iterate over the completed tasks and remove them from
                    // the task queue
                    for (Task* t : completed)
                    {
                        t->notifyDependents();
                        auto itr = find(mTaskQueue.begin(),mTaskQueue.end(), t);
                        if (itr != mTaskQueue.end())
                        {
                            LOG_TRACE("TaskThread: Task {}({}) is being popped off thread {}'s queue",
                                      t->getTaskName(), t->getTaskId(), mThreadId);
                            mTaskQueue.erase(itr);
                        }
                    }
                    completed.clear();
                }
                mTaskQueueMutex.unlock();
            }

            // Process the destruction task queue
            if(mDestructionTaskQueueMutex.try_lock())
            {
                vector<shared_ptr<DestructionTask>> completed;
                for (auto itr = mDestructionTaskQueue.begin();
                     itr != mDestructionTaskQueue.end(); itr++)
                {
                    shared_ptr<DestructionTask>& t = (*itr);
                    if (t->isWaitingForDependencies())
                    {
                        t->incrementDeferralCount();
                    }
                    else
                    {
                        t->setState(TaskState::TASK_STATE_ACTIVE);
                        t->execute();
                    }

                    if (t->getState() == TaskState::TASK_STATE_COMPLETED)
                    {
                        completed.push_back(t);
                    }
                }

                // Remove completed DestructionTasks from queue
                for (shared_ptr<DestructionTask>& t : completed)
                {
                    t->notifyDependents();
                    auto itr = find(mDestructionTaskQueue.begin(),
                                    mDestructionTaskQueue.end(), t);

                    if (itr != mDestructionTaskQueue.end())
                    {
                        mDestructionTaskQueue.erase(itr);
                    }
                }
                mDestructionTaskQueueMutex.unlock();
            }

            LOG_TRACE("TaskThread: Worker {} has finished running it's task queue, Setting Fence",getThreadId());
            mFence = true;
            std::this_thread::yield();
        }
        LOG_TRACE("TaskThread: ---------- Worker {} is ending it's task queue",getThreadId());
    }

    void TaskThread::clearFence()
    {
        if (!mFence)
        {
            LOG_ERROR("TaskThread: Thread {} attempted to clear an unset fence",getThreadId());
            LOG_ERROR("TaskThread: has the following Tasks (count == {})",getThreadId(), mTaskQueue.size());
            for (Task* t: mTaskQueue)
            {
                LOG_ERROR("\tID:{} Name:{}",t->getTaskId(),t->getTaskName());
            }
            LOG_ERROR("TaskThread: has the following DestructionTasks (count == {})",getThreadId(), mDestructionTaskQueue.size());
            for (shared_ptr<DestructionTask>& t: mDestructionTaskQueue)
            {
                LOG_ERROR("\tID:{} Name:{}",t->getTaskId(),t->getTaskName());
            }
        }
        assert(mFence);
        mFence = false;
        LOG_TRACE("TaskThread: Cleared fence for thread {}",getThreadId());
    }

    bool TaskThread::getFence()
    {
        return mFence;
    }

    bool TaskThread::pushTask(Task* t)
    {
        if(mTaskQueueMutex.try_lock())
        {
            assert(mFence);
            mTaskQueue.push_back(t);
            assert(mThreadId >= 0);
            t->setThreadId(mThreadId);
            mTaskQueueMutex.unlock();
            return true;
        }
        return false;
    }

    bool TaskThread::pushDestructionTask(const shared_ptr<DestructionTask>& dt)
    {
        if(mDestructionTaskQueueMutex.try_lock())
        {
            assert(mFence);
            dt->setThreadId(mThreadId);
            mDestructionTaskQueue.push_back(dt);
            mDestructionTaskQueueMutex.unlock();
            return true;
        }
        return false;
    }

    void TaskThread::setRunning(bool running)
    {
        mRunning = running;
    }

    int TaskThread::getThreadId()
    {
        return mThreadId;
    }

    const vector<Task*>& TaskThread::getDebugTaskQueue()
    {
        sort(mDebugTaskQueue.begin(),mDebugTaskQueue.end(),
             [&](Task* t1, Task* t2) {return t1->getTaskId() > t2->getTaskId();});
        return mDebugTaskQueue;
    }

    bool TaskThread::hasTask(Task *t)
    {
        bool found = false;
        mTaskQueueMutex.lock();
		auto itr = find(mTaskQueue.begin(), mTaskQueue.end(), t);
        if (itr != mTaskQueue.end()) found = true;
        mTaskQueueMutex.unlock();
        return found;
    }
}
