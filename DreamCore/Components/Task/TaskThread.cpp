#include "TaskThread.h"
#include "Task.h"
#include "Common/Logger.h"

#include <algorithm>

using std::find;
using std::unique_lock;

namespace octronic::dream
{

    TaskThread::TaskThread (int id)
        : LockableObject("TaskThread"),
          mThread(thread(&TaskThread::executeTaskQueue,this)),
          mRunning(false),
          mWaitingToRunFence(true),
          mThreadID(id)
    {
        assert(mThreadID > Task::INVALID_THREAD_ID);
    }

    void TaskThread::join()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();

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
                std::this_thread::yield();
                continue;
            }

            // Process the logical task queue ==================================

            const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        	if (!lg.owns_lock()) getMutex().lock();

            while(!mTaskQueue.empty())
            {
                vector<Task*> completed;
                LOG_TRACE("TaskThread[{}]: Thread has {} tasks",getThreadID(),mTaskQueue.size());

                for (auto itr = mTaskQueue.begin(); itr != mTaskQueue.end(); ++itr)
                {
                    Task* task = (*itr);
                    LOG_INFO("TaskThread[{}]: Processing task {}",getThreadID(),task->getDebugString());
                    // Check if ready to execute
                    try
                    {
                        switch(task->getState())
                        {
                            case TASK_STATE_CONSTRUCTED:
                                LOG_ERROR("TaskThread[{}]: Task {} constructed, should not have been pushed to queue", getThreadID(),task->getDebugString());
                                assert(false);
                                break;
                            // Task failed
                            case TaskState::TASK_STATE_FAILED:
                            {
                                LOG_TRACE("TaskThread[{}]: Task {} FAILED, welp", getThreadID(), task->getDebugString());
                                assert(false);
                                break;
                            }
                            // Not waiting for dependencies, so mark active and
                            // execute
                            case TaskState::TASK_STATE_QUEUED:
                            {
                                if (!task->isWaitingForDependencies())
								{
                                    LOG_TRACE("TaskThread[{}]: Task {} is ready", getThreadID(), task->getDebugString());
                                	assert(task->getThreadID() > Task::INVALID_THREAD_ID);
									task->setState(TaskState::TASK_STATE_ACTIVE);
									task->execute();
								}
                                else
                                {
                                    LOG_TRACE("TaskThread[{}]: Task {} is waiting",getThreadID(), task->getDebugString());
									task->incrementDeferralCount();
                                }
                                break;
                            }
                            // Task was completed, i.e. not deferred and not failed
							case TaskState::TASK_STATE_COMPLETED:
							{
								LOG_TRACE("TaskThread[{}]: Task {} was completed", getThreadID(), task->getDebugString());
								task->notifyTasksWaitingForMe();
								completed.push_back(task);
                                break;
							}
							case TASK_STATE_ACTIVE:
                            {
								LOG_TRACE("TaskThread[{}]: Task {} is active... Retrying task", getThreadID(), task->getDebugString());
                                // retry execution
                                task->execute();
                                break;
                            }
                        }
                    }
                    catch (const std::exception& ex)
                    {
                        LOG_ERROR("TaskThread[{}]: Exception with task {}",getThreadID(), ex.what());
                        task->incrementDeferralCount();
                        //task->setState(TaskState::TASK_STATE_FAILED);
                    }
                }

                // Iterate over the completed tasks and remove them from
                // the task queue
                for (Task* t : completed)
                {
                    auto itr = find(mTaskQueue.begin(),mTaskQueue.end(), t);
                    if (itr != mTaskQueue.end())
                    {
                        LOG_TRACE("TaskThread[{}]: Task {} is being popped off the queue", getThreadID(), t->getDebugString());
                        mTaskQueue.erase(itr);
                    }
                    else
                    {
                        LOG_ERROR("TaskThread[{}]: Error, Processed task was not in the queue to remove?", getThreadID());
                        assert(false);
                    }
                }
                completed.clear();
            }

            // Process the destruction task queue ==============================

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
                    t->notifyTasksWaitingForMe();
                    completed.push_back(t);
                }
            }

            // Remove completed DestructionTasks from queue
            for (shared_ptr<DestructionTask>& t : completed)
            {
                auto itr = find(mDestructionTaskQueue.begin(),
                                mDestructionTaskQueue.end(), t);

                if (itr != mDestructionTaskQueue.end())
                {
                    mDestructionTaskQueue.erase(itr);
                }
            }

            LOG_TRACE("TaskThread[{}]: Thread has finished it's task queue, Setting Fence",getThreadID());
            mWaitingToRunFence = true;
            std::this_thread::yield();
        }
    }

    void TaskThread::clearWaitingToRunFence()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();

        if (!mWaitingToRunFence)
        {
            LOG_ERROR("TaskThread[{}]: Attempted to clear an unset fence",getThreadID());
            LOG_ERROR("TaskThread[{}]: Has the following Tasks (count == {})",getThreadID(), mTaskQueue.size());
            for (Task* t: mTaskQueue)
            {
                LOG_ERROR("\t{}",t->getDebugString());
            }
            LOG_ERROR("TaskThread[{}]: Has the following DestructionTasks (count == {})",getThreadID(), mDestructionTaskQueue.size());
            for (shared_ptr<DestructionTask>& t: mDestructionTaskQueue)
            {
                LOG_ERROR("\t{}",t->getDebugString());
            }
        }
        assert(mWaitingToRunFence);
        mWaitingToRunFence = false;
        LOG_TRACE("TaskThread[{}]: Cleared fence",getThreadID());
    }

    bool TaskThread::getWaitingToRunFence()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        LOG_TRACE("TaskThread[{}]: {}",getThreadID(),__FUNCTION__);

        if (mTaskQueue.empty())
        {
            LOG_TRACE("TaskThread[{}]:\tTaskQueue is empty :)",getThreadID());
        }
        for(Task* task : mTaskQueue)
        {
            LOG_TRACE("TaskThread[{}]:\t{} state: {}",getThreadID(),task->getDebugString(), Task::stateToString(task->getState()));
        }

        return mWaitingToRunFence;
    }

    bool TaskThread::pushTask(Task* t)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();

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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();

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
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mRunning = running;
    }

    int TaskThread::getThreadID()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        return mThreadID;
    }

    bool TaskThread::hasTask(Task *t)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        auto itr = find(mTaskQueue.begin(), mTaskQueue.end(), t);
        if (itr != mTaskQueue.end()) return true;
        return false;
    }


    size_t TaskThread::getNumTasks()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        return mTaskQueue.size();
    }
}
