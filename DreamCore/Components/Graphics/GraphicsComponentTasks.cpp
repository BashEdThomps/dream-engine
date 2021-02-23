#include "GraphicsComponentTasks.h"

#include "GraphicsComponent.h"
#include "Project/ProjectRuntime.h"

#include <sstream>
using std::stringstream;

namespace octronic::dream
{
    // GraphicsComponentTask ===================================================

    GraphicsTask::GraphicsTask(ProjectRuntime* pr, const string& taskName)
        : Task(pr, taskName)
    {

    }

    // GraphicsComponentDestructionTask ========================================

    GraphicsDestructionTask::GraphicsDestructionTask
    (ProjectRuntime* pr, const string& taskName)
        : DestructionTask(pr, taskName)
    {

    }

    // SetupBuffersTask ========================================================

    SetupBuffersTask::SetupBuffersTask(ProjectRuntime* pr)
        : GraphicsTask(pr, "SetupBuffersTask")
    {
    }

    void SetupBuffersTask::execute()
    {
        auto graphicsComponent = mProjectRuntimeHandle->getGraphicsComponent();
        if (graphicsComponent->setupBuffers())
        {
           setState(TASK_STATE_COMPLETED);
        }
        else
        {
            setState(TASK_STATE_DEFERRED);
        }
    }

    // HandleResizeTask ========================================================

    HandleResizeTask::HandleResizeTask(ProjectRuntime* pr)
        : GraphicsTask(pr, "HandleResizeTask")
    {
    }

    void HandleResizeTask::execute()
    {
        auto graphicsComponent = mProjectRuntimeHandle->getGraphicsComponent();
        if (graphicsComponent->handleResize())
        {
           setState(TASK_STATE_COMPLETED);
        }
        else
        {
            setState(TASK_STATE_FAILED);
        }
    }

    // RenderTask ==============================================================

    RenderTask::RenderTask(ProjectRuntime* pr)
        : GraphicsTask(pr, "RenderTask")
    {
    }

    void RenderTask::execute()
    {
        auto graphicsComponent = mProjectRuntimeHandle->getGraphicsComponent();
        auto sr = mProjectRuntimeHandle->getActiveSceneRuntime();
        if (sr!=nullptr)
        {
            graphicsComponent->clearBuffers(sr);
			graphicsComponent->renderModels(sr);
            graphicsComponent->renderEnvironment(sr);
			//graphicsComponent->renderShadowPass(sr);
			//graphicsComponent->renderSprites(sr);
			//graphicsComponent->renderFonts(sr);

        }
        setState(TASK_STATE_COMPLETED);
    }
}
