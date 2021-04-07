#pragma once

#include "TaskState.h"

#include <functional>
#include <string>
#include <vector>

using std::vector;
using std::string;
using std::reference_wrapper;

namespace octronic::dream
{
  class ProjectRuntime;

  /**
     * @brief Task is the base class of any Runtime Logic task, allowing
     * logic blocks to be dispatched to the TaskManager.
     *
     * *** A Task should ONLY CALL logic functions, not implement them
     *
     * *** A DestructionTask MAY implement some logic, as the parent
     *     object may have been destroyed.
     */
  class Task
  {
  public:
    static int TaskIDGenerator;
    const static int INVALID_THREAD_ID;
    const static int INVALID_TASK_ID;

    Task(ProjectRuntime& pr,  const string& taskName);
    virtual ~Task();

    Task(const Task&) = delete;
    Task& operator=(const Task&) = delete;

    Task(Task&&) = default;
    Task& operator=(Task&&) = default;

    virtual void execute() = 0;

    int getID() const;
    string getName() const;
    virtual string getNameAndIDString() const;

    void setState(const TaskState& s);
    TaskState getState() const;
    bool hasState(const TaskState& s) const;

    bool operator==(Task& other) const;

  public: // Statics
    static int taskIDGenerator();

  protected:
    ProjectRuntime& getProjectRuntime() const;
  private:
    reference_wrapper<ProjectRuntime> mProjectRuntime;

    int mID;
    string mName;
    TaskState mState;
  };

  /**
     * @brief The DestructionTask class is used for tasks that need to exist beyond
     * their owner's scope. These tasks are copied into the TaskThread's Execution
     * Queue instead of being called by pointer. They will typically be used for
     * clean-up operations after their parent object has been destroyed, or is otherwise
     * no longer available.
     *
     * Must be instanciated on stack as shared pointers
     */
  class DestructionTask : public Task
  {
  public:
    DestructionTask(ProjectRuntime& pr, const string& taskName);
  };
}
