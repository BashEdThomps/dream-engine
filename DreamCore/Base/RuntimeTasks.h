#pragma once

#include "Task/Task.h"

namespace octronic::dream
{
	class Runtime;

	class RuntimeLoadFromDefinitionTask : public Task
	{
	public:
		RuntimeLoadFromDefinitionTask(ProjectRuntime* pr, Runtime* runtime);
		void execute() override;
        string getNameAndIDString() override;
	private:
		Runtime* mRuntime;
	};
}
