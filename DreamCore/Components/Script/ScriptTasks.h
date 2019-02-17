#pragma once

#include "../../TaskManager/Task.h"
#include "../Graphics/GraphicsComponentTask.h"

namespace Dream
{
    class SceneObjectRuntime;
    class ScriptRuntime;

    class ScriptConstructionTask : public Task
    {
        ScriptRuntime* mScript = nullptr;
    public:
        ScriptConstructionTask();
        void execute() override;
        void setScript(ScriptRuntime* rt);
    };

    class ScriptOnInitTask : public Task
    {
        SceneObjectRuntime* mSceneObject;
        ScriptRuntime* mScript = nullptr;

    public:
        ScriptOnInitTask(SceneObjectRuntime* rt);
        void execute() override;
        void setScript(ScriptRuntime* rt);
    };

    class ScriptOnUpdateTask : public Task
    {
        SceneObjectRuntime* mSceneObject;
        ScriptRuntime* mScript = nullptr;

    public:
        ScriptOnUpdateTask(SceneObjectRuntime* rt);
        void execute() override;
        void setScript(ScriptRuntime* rt);
    };

    class ScriptOnEventTask : public Task
    {
        SceneObjectRuntime* mSceneObject;
        ScriptRuntime* mScript = nullptr;

    public:
        ScriptOnEventTask(SceneObjectRuntime* rt);
        void execute() override;
        void setScript(ScriptRuntime* rt);
    };

    class ScriptOnDestroyTask : public DestructionTask
    {
        SceneObjectRuntime* mParentSceneObject;
        ScriptRuntime* mScript = nullptr;
        uint32_t mDestroyedObject;
    public:
        ScriptOnDestroyTask(uint32_t destroyed, SceneObjectRuntime* rt);
        void execute() override;
        void setScript(ScriptRuntime* rt);
    };

    class ScriptOnNanoVGTask : public GraphicsComponentTask
    {
        SceneObjectRuntime* mSceneObject;
        ScriptRuntime* mScript = nullptr;

    public:
        ScriptOnNanoVGTask(SceneObjectRuntime* rt);
        void execute();
        void setScript(ScriptRuntime* rt);
    };
}
