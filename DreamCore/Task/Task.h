#pragma once

#include "TaskState.h"

#include <string>
#include <vector>
#include <map>
#include <memory>

using std::shared_ptr;
using std::weak_ptr;
using std::enable_shared_from_this;
using std::map;
using std::vector;
using std::string;

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
    class Task : public enable_shared_from_this<Task>
    {
    public:
        static int TaskIDGenerator;
        const static int INVALID_THREAD_ID;
        const static int INVALID_TASK_ID;

        Task(const weak_ptr<ProjectRuntime>& pr,  const string& taskName);
        virtual ~Task();

        virtual void execute() = 0;

        int getID() const;
        string getName() const;
        virtual string getNameAndIDString() const;

        void clearState();
        void setState(const TaskState& s);
        TaskState getState() const;
        bool hasState(const TaskState& s) const;

        bool operator==(Task& other) const;

    public: // Statics
        static int taskIDGenerator();

    protected:
		weak_ptr<ProjectRuntime> mProjectRuntime;

    private:
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
        DestructionTask(const weak_ptr<ProjectRuntime>& pr, const string& taskName);
    };
}
