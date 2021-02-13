#include "LightTasks.h"

#include "LightRuntime.h"
#include "Project/ProjectRuntime.h"
#include "Components/Graphics/GraphicsComponent.h"

namespace octronic::dream
{
	LightAddToQueueTask::LightAddToQueueTask(ProjectRuntime* pr, LightRuntime* lr)
        : GraphicsTask(pr,"LightAddToQueueTask"),
          mLightRuntime(lr)

    {

    }

    void LightAddToQueueTask::execute()
    {
       auto graphicsComp = mProjectRuntime->getGraphicsComponent();
       graphicsComp->addToLightQueue(mLightRuntime->getEntityRuntimeHandle());
       setState(TASK_STATE_COMPLETED);
    }
}
