#include "TaskThread.h"

#include "Task.h"
#include "Common/Logger.h"
#include "Base/ThreadLockFailedException.h"

#include <algorithm>

using std::find;

namespace octronic::dream
{
    TaskThread::TaskThread (int id)
        : mThread(thread(&TaskThread::executeTaskQueue,this)),
          mRunning(false),
          mWaitingToRunFence(true),
          mThreadID(id)
    {
        assert(mThreadID > Task::INVALID_THREAD_ID);
    }

    void TaskThread::join()
    {
        LOG_TRACE("TaskThread[{}]: Joining Thread",getThreadID());
        mThread.join();

    }

    void TaskThread::executeTaskQueue()
    {
        mRunning = true;
        while (mRunning)
        {
            if (mWaitingToRunFence)
            {
                continue;
            }

            // Process the task queue ==========================================

            vector<Task*> completed;
            LOG_TRACE("TaskThread[{}]: Thread has {} tasks",getThreadID(),mTaskQueue.size());

            for (auto itr = mTaskQueue.begin(); itr != mTaskQueue.end(); ++itr)
            {
                Task* task = (*itr);
                LOG_INFO("TaskThread[{}]: Processing task {}",getThreadID(),task->getTaskName());
                // Check if ready to execute
                switch(task->getState())
                {
                    case TASK_STATE_CONSTRUCTED:
                        LOG_ERROR("TaskThread[{}]: Task {} constructed, should not have been pushed to queue", getThreadID(),task->getTaskName());
                        assert(false);
                        break;
                        // Task failed
                    case TaskState::TASK_STATE_FAILED:
                    {
                        LOG_TRACE("TaskThread[{}]: Task {} FAILED, welp", getThreadID(), task->getTaskName());
                        assert(false);
                        break;
                    }
                        // Not waiting for dependencies, so mark active and
                        // execute
                    case TaskState::TASK_STATE_QUEUED:
                    {
                        if (!task->isWaitingForDependencies())
                        {
                            LOG_TRACE("TaskThread[{}]: Task {} is ready", getThreadID(), task->getTaskName());
                            assert(task->getThreadID() > Task::INVALID_THREAD_ID);
                            task->setState(TaskState::TASK_STATE_ACTIVE);
                            try
                            {
                                task->execute();
                            }
                            catch (ThreadLockFailedException ex)
                            {
                                task->incrementDeferralCount();
                            }
                        }
                        else
                        {
                            LOG_TRACE("TaskThread[{}]: Task {} is waiting",getThreadID(), task->getTaskName());
                            task->incrementDeferralCount();
                        }
                        break;
                    }
                        // Task was completed, i.e. not deferred and not failed
                    case TaskState::TASK_STATE_COMPLETED:
                    {
                        LOG_TRACE("TaskThread[{}]: Task {} was completed", getThreadID(), task->getTaskName());
                        task->notifyTasksWaitingForMe();
                        completed.push_back(task);
                        break;
                    }
                        // Was run but previously failed, can still complete
                    case TASK_STATE_ACTIVE:
                    {
                        LOG_TRACE("TaskThread[{}]: Task {} is active... Retrying task", getThreadID(), task->getTaskName());
                        // retry execution
                        try
                        {
                            task->execute();
                        }
                        catch (ThreadLockFailedException ex)
                        {
                            task->incrementDeferralCount();
                        }
                        break;
                    }

                }
            }

            // Iterate over the completed tasks and remove them from
            // the task queue
            for (Task* t : completed)
            {
                auto itr = find(mTaskQueue.begin(),mTaskQueue.end(), t);
                if (itr != mTaskQueue.end())
                {
                    LOG_TRACE("TaskThread[{}]: Task {} is being popped off the queue", getThreadID(), t->getTaskName());
                    mTaskQueue.erase(itr);
                }
                else
                {
                    LOG_ERROR("TaskThread[{}]: Error, Processed task was not in the queue to remove?", getThreadID());
                    assert(false);
                }
            }
            completed.clear();


            // Process the destruction task queue ==============================
            vector<shared_ptr<DestructionTask>> destructionCompleted;
            for (auto itr = mDestructionTaskQueue.begin(); itr != mDestructionTaskQueue.end(); itr++)
            {
                shared_ptr<DestructionTask>& t = (*itr);
                if (t->isWaitingForDependencies())
                {
                    t->incrementDeferralCount();
                }
                else
                {
                    t->setState(TaskState::TASK_STATE_ACTIVE);
                    try
                    {
                        t->execute();
                    }
                    catch (ThreadLockFailedException ex)
                    {
                        t->incrementDeferralCount();
                    }
                }

                if (t->getState() == TaskState::TASK_STATE_COMPLETED)
                {
                    t->notifyTasksWaitingForMe();
                    destructionCompleted.push_back(t);
                }


            }

            // Remove completed DestructionTasks from queue
            for (shared_ptr<DestructionTask>& t : destructionCompleted)
            {
                auto itr = find(mDestructionTaskQueue.begin(),mDestructionTaskQueue.end(), t);

                if (itr != mDestructionTaskQueue.end())
                {
                    mDestructionTaskQueue.erase(itr);
                }
            }

            LOG_TRACE("TaskThread[{}]: Thread has finished it's task queue, Setting Fence",getThreadID());
            mWaitingToRunFence = true;
        }
    }

    void TaskThread::clearWaitingToRunFence()
    {
        if (!mWaitingToRunFence)
        {
            LOG_ERROR("TaskThread[{}]: Attempted to clear an unset fence",getThreadID());
            LOG_ERROR("TaskThread[{}]: Has the following Tasks (count == {})",getThreadID(), mTaskQueue.size());
            for (Task* t: mTaskQueue)
            {
                LOG_ERROR("\t{}",t->getTaskName());
            }
            LOG_ERROR("TaskThread[{}]: Has the following DestructionTasks (count == {})",getThreadID(), mDestructionTaskQueue.size());
            for (shared_ptr<DestructionTask>& t: mDestructionTaskQueue)
            {
                LOG_ERROR("\t{}",t->getTaskName());
            }
        }
        assert(mWaitingToRunFence);
        mWaitingToRunFence = false;
        LOG_TRACE("TaskThread[{}]: Cleared fence",getThreadID());

    }

    bool TaskThread::getWaitingToRunFence() const
    {
        /*
        LOG_TRACE("TaskThread[{}]: {}",getThreadID(),__FUNCTION__);

        if (mTaskQueue.empty())
        {
            LOG_TRACE("TaskThread[{}]:\tTaskQueue is empty :)",getThreadID());
        }
        for(Task* task : mTaskQueue)
        {
            LOG_TRACE("TaskThread[{}]:\t{} state: {}",getThreadID(),task->getTaskName(), Task::stateToString(task->getState()));
        }
        */

        return mWaitingToRunFence;
    }

    bool TaskThread::pushTask(Task* t)
    {
        LOG_TRACE("TaskThread[{}]: {}", getThreadID(), __FUNCTION__);

        assert(mThreadID > Task::INVALID_THREAD_ID);

        assert(mWaitingToRunFence);
        t->clearState();
        t->setState(TaskState::TASK_STATE_QUEUED);
        t->setThreadID(mThreadID);
        mTaskQueue.push_back(t);
        return true;
    }

    bool TaskThread::pushDestructionTask(const shared_ptr<DestructionTask>& dt)
    {
        LOG_TRACE("TaskThread[{}]: {}", getThreadID(), __FUNCTION__);

        assert(dt->getThreadID() == Task::INVALID_THREAD_ID);
        assert(mThreadID > Task::INVALID_THREAD_ID);

        assert(mWaitingToRunFence);
        dt->clearState();
        dt->setThreadID(mThreadID);
        dt->setState(TaskState::TASK_STATE_QUEUED);
        mDestructionTaskQueue.push_back(dt);
        return true;
    }

    void TaskThread::setRunning(bool running)
    {
        mRunning = running;
    }

    int TaskThread::getThreadID() const
    {
        return mThreadID;
    }

    bool TaskThread::hasTask(Task *t)
    {
        auto itr = find(mTaskQueue.begin(), mTaskQueue.end(), t);
        if (itr != mTaskQueue.end()) return true;
        return false;
    }


    size_t TaskThread::getNumTasks() const
    {
        return mTaskQueue.size();
    }
}
