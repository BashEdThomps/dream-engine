#pragma once

#include "Components/Graphics/GraphicsComponentTasks.h"

namespace octronic::dream
{
	class LightRuntime;
	class LightAddToQueueTask : public GraphicsTask
	{
	public:
		LightAddToQueueTask(ProjectRuntime* pr, LightRuntime* lr);
		void execute() override;
	private:
		LightRuntime* mLightRuntime;
	};
}
