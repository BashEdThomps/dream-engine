#pragma once

#include "Task/Task.h"

namespace octronic::dream
{
    // Construction Task =======================================================

    class GraphicsTask : public Task
    {
    public:
        GraphicsTask(ProjectRuntime* pr, const string& taskName);
    };

    // Destruction Task ========================================================

    class GraphicsDestructionTask : public DestructionTask
    {
    public:
        GraphicsDestructionTask(ProjectRuntime* pr, const string& taskName);
    };


    // SetupBuffersTask ========================================================

    class SetupBuffersTask : public GraphicsTask
    {
    public:
        SetupBuffersTask(ProjectRuntime* pr);
		void execute() override;
    };

	// HandleResizeTask ========================================================

    class HandleResizeTask : public GraphicsTask
    {
    public:
        HandleResizeTask(ProjectRuntime* pr);
		void execute() override;
    };

	// RenderTask ========================================================

    class RenderTask : public GraphicsTask
    {
    public:
        RenderTask(ProjectRuntime* pr);
		void execute() override;
    };
}
