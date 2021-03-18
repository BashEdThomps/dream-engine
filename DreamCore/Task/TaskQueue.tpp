#pragma once

#include "TaskState.h"
#include "Common/Logger.h"

namespace octronic::dream
{
  template <typename TaskType>
  TaskQueue<TaskType>::TaskQueue
  (const string& className)
    : mClassName(className)
  {

  }

  template <typename TaskType>
  void
  TaskQueue<TaskType>::pushTask
  (const weak_ptr<TaskType>& task)
  {
    if (auto taskLock = task.lock())
    {
      LOG_TRACE("{}: {} {}", mClassName, __FUNCTION__, taskLock->getNameAndIDString());
      if (!hasTask(task))
      {
        taskLock->clearState();
        mQueue.push_back(task);
      }
      else if (!taskLock->hasState(TASK_STATE_DEFERRED))
      {
        assert(false);
      }
    }
  }

  template <typename TaskType>
  void
  TaskQueue<TaskType>::executeQueue
  ()
  {
    vector<weak_ptr<TaskType>> completed;

    // Process the task queue ==========================================
    LOG_TRACE("{}: has {} tasks", mClassName, mQueue.size());

    for (auto itr = mQueue.begin(); itr != mQueue.end(); itr++)
    {
      weak_ptr<TaskType> task = (*itr);
      if (auto taskLock = task.lock())
      {
        LOG_INFO("{}: Processing task {}", mClassName, taskLock->getNameAndIDString());

        if (taskLock->hasState(TASK_STATE_QUEUED) || taskLock->hasState(TASK_STATE_DEFERRED))
        {
          LOG_TRACE("{}: Task {} is queued... executing", mClassName,  taskLock->getNameAndIDString());
          // try execution
          taskLock->execute();
          if (taskLock->hasState(TASK_STATE_COMPLETED))
          {
            LOG_TRACE("{}: Task {} was completed", mClassName,  taskLock->getNameAndIDString());
            completed.push_back(task);
          }
          else if (taskLock->hasState(TASK_STATE_FAILED))
          {
            // Saturate the log before assertion  :'(
            for (int i=0; i<100;i++)
            {
              LOG_TRACE("{}: Task {} FAILED", mClassName,  taskLock->getNameAndIDString());
            }
            assert(false);
          }
        }
      }
    }

    // Iterate over the completed tasks and remove them from
    // the task queue
    for (auto itr = completed.begin(); itr != completed.end(); itr++)
    {
      weak_ptr<TaskType> t = (*itr);

      if (auto taskLock = t.lock())
      {
        auto q_itr = find_if(
              mQueue.begin(),mQueue.end(),
              [&](const weak_ptr<TaskType>& next)
              {return next.lock() == taskLock;});

        if (q_itr != mQueue.end())
        {
          LOG_TRACE("{}: Task {} is being popped off the queue", mClassName,  t.lock()->getNameAndIDString());
          mQueue.erase(q_itr);
        }
        else
        {
          LOG_ERROR("{}: Error, Processed \"{}\"task was not in the queue to remove?", taskLock->getNameAndIDString(), mClassName);
          assert(false);
        }
      }
    }
    completed.clear();

    LOG_TRACE("{}: Thread has finished it's task queue",  mClassName);
  }

  template <typename TaskType>
  bool
  TaskQueue<TaskType>::hasTask
  (const weak_ptr<TaskType>& t)
  const
  {
    auto itr = find_if(mQueue.begin(), mQueue.end(),
                       [&](const weak_ptr<TaskType>& fromVec)
    { return fromVec.lock() == t.lock(); });

    return itr != mQueue.end();
  }

  template <typename TaskType>
  size_t
  TaskQueue<TaskType>::getTaskCount
  ()
  const
  {
    return mQueue.size();
  }
}
