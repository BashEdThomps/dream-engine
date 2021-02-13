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
        auto graphicsComponent = mProjectRuntime->getGraphicsComponent();
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
        auto graphicsComponent = mProjectRuntime->getGraphicsComponent();
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
        auto graphicsComponent = mProjectRuntime->getGraphicsComponent();
        auto sr = mProjectRuntime->getActiveSceneRuntime();
        if (sr!=nullptr)
        {
			graphicsComponent->renderGeometryPass(sr);
			graphicsComponent->renderShadowPass(sr);
			graphicsComponent->renderLightingPass(sr);
			graphicsComponent->renderSprites(sr);
			graphicsComponent->renderFonts(sr);
			graphicsComponent->clearLightQueue();
        }
        setState(TASK_STATE_COMPLETED);
    }
}
