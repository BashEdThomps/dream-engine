#pragma once

#include "TaskState.h"
#include "TaskQueue.h"
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
  (const shared_ptr<TaskType>& task)
  {
    LOG_TRACE("{}: {} {}", mClassName, __FUNCTION__, task->getNameAndIDString());
    if (!hasTask(task))
    {
    	task->setState(TASK_STATE_QUEUED);
      mQueue.push_back(task);
    }
    else if (!task->hasState(TASK_STATE_DEFERRED))
    {
      assert(false);
    }
  }

  template <typename TaskType>
  void
  TaskQueue<TaskType>::executeQueue
  ()
  {
    vector<shared_ptr<TaskType>> completed;

    // Process the task queue ==========================================
    LOG_TRACE("{}: has {} tasks", mClassName, mQueue.size());

    for (auto itr = mQueue.begin(); itr != mQueue.end(); itr++)
    {
      shared_ptr<TaskType> task = (*itr);
      LOG_INFO("{}: Processing task {}", mClassName, task->getNameAndIDString());

      if (task->hasState(TASK_STATE_QUEUED) || task->hasState(TASK_STATE_DEFERRED))
      {
        LOG_TRACE("{}: Task {} is queued... executing", mClassName,
                  task->getNameAndIDString());

        // try execution
        task->execute();

        if (task->hasState(TASK_STATE_COMPLETED))
        {
          LOG_TRACE("{}: Task {} was completed", mClassName,  task->getNameAndIDString());
          completed.push_back(task);
        }
        else if (task->hasState(TASK_STATE_FAILED))
        {
          LOG_ERROR("{}: Task {} FAILED", mClassName,  task->getNameAndIDString());
          assert(false);
        }
      }
    }

    // Iterate over the completed tasks and remove them from
    // the task queue
    for (auto itr = completed.begin(); itr != completed.end(); itr++)
    {
      shared_ptr<TaskType>& task = (*itr);

      auto q_itr = find(mQueue.begin(),mQueue.end(),task);

      if (q_itr != mQueue.end())
      {
        LOG_TRACE("{}: Task {} is being popped off the queue", mClassName,
                  task->getNameAndIDString());
        mQueue.erase(q_itr);
      }
      else
      {
        LOG_ERROR("{}: Error, Processed \"{}\"task was not in the queue to remove?",
                  task->getNameAndIDString(), mClassName);
        assert(false);
      }
    }

    LOG_TRACE("{}: Thread has finished it's task queue",  mClassName);
  }

  template <typename TaskType>
  bool
  TaskQueue<TaskType>::hasTask
  (const shared_ptr<TaskType>& t)
  const
  {
    auto itr = find_if(mQueue.begin(), mQueue.end(),
                       [&](const shared_ptr<TaskType>& fromVec)
    { return fromVec.get() == t.get(); } );
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
