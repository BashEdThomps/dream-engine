#pragma once

#include "Task/Task.h"

namespace octronic::dream
{
	class DeferredLoadRuntime;
    class ProjectRuntime;
    class DeferredLoadRuntime;

	class RuntimeLoadFromDefinitionTask : public Task
	{
	public:
		RuntimeLoadFromDefinitionTask(
                const weak_ptr<ProjectRuntime>& pr,
                const weak_ptr<DeferredLoadRuntime>& runtime);
		void execute() override;
        string getNameAndIDString() const override;
	private:
		weak_ptr<DeferredLoadRuntime> mRuntime;
	};
}
