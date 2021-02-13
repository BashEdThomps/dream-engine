#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace octronic::dream
{
    /**
     * @brief The TaskQueue class is responsible for scheduling and executing
     * Tasks that are used to implement a Project's runtime logic.
     *
     * AssetRuntime Objects will provide tasks in the following way:
     *
     *	When Loading: Pushes a LoadFromDefinitionTask
     *  In-Flight: Pushes a 'Persistent' UpdateTask
     *	When Done: Pushes a DestructionTask
     *
     * Tasks are held by the Owning Object and deleted by the Owning Object.
     * TaskQueue will NOT delete tasks marked as Persisteent as they are to be
     * performed repeatedly.
     *
     * ProjectRuntime will submit all Tasks for SharedAssetInstances held by
     * their respective Components.
     *
     * for each component:
     *     for each runtime:
     *          // will push the correct task for the runtime's state
     *			runtime->pushNextTask()
     *
     * SceneRuntime will submit all Tasks for DiscreteAssetInstances owned by
     * Entities the Entities tree.
     *
     * Graphics-related tasks MUST BE executed on the main thread by the
     * GraphicsComponent (OpenGL threading limitation, yes I know about Vulcan).
     */
	template <typename TaskType>
	class TaskQueue
	{
	public:
		TaskQueue(const string& className);
		void pushTask(const TaskType& t);
		void executeQueue();
		bool hasTask(const TaskType& t) const;
		vector<TaskType>* getTaskQueue() const;
		size_t getTaskCount() const;
	private:
		vector<TaskType> mQueue;
        string mClassName;
	};
}

// Template Implementation
#include "TaskQueue.tpp"
