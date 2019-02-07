#pragma once

#include "../../TaskManager/Task.h"
#include "../Graphics/GraphicsComponentTask.h"

namespace Dream
{
    class SceneObjectRuntime;
    class ScriptRuntime;

    class ScriptCreateStateTask : public Task
    {
        SceneObjectRuntime* mSceneObject;
        ScriptRuntime* mScript;
    public:
        ScriptCreateStateTask(SceneObjectRuntime* rt);
        void execute();
        void setScript(ScriptRuntime* rt);
    };


    class ScriptRemoveStateTask : public Task
    {
        uint32_t mUuid;
        ScriptRuntime* mScript;
    public:
        ScriptRemoveStateTask(uint32_t uuid);
        void execute();
        void setScript(ScriptRuntime* rt);
    };

    class ScriptOnInitTask : public Task
    {
        SceneObjectRuntime* mSceneObject;
        ScriptRuntime* mScript;

    public:
        ScriptOnInitTask(SceneObjectRuntime* rt);
        void execute();
        void setScript(ScriptRuntime* rt);
    };

    class ScriptOnUpdateTask : public Task
    {
        SceneObjectRuntime* mSceneObject;
        ScriptRuntime* mScript;

    public:
        ScriptOnUpdateTask(SceneObjectRuntime* rt);
        void execute();
        void setScript(ScriptRuntime* rt);
    };

    class ScriptOnEventTask : public Task
    {
        SceneObjectRuntime* mSceneObject;
        ScriptRuntime* mScript;

    public:
        ScriptOnEventTask(SceneObjectRuntime* rt);
        void execute();
        void setScript(ScriptRuntime* rt);
    };

    class ScriptOnNanoVGTask : public GraphicsComponentTask
    {
        SceneObjectRuntime* mSceneObject;
        ScriptRuntime* mScript;

    public:
        ScriptOnNanoVGTask(SceneObjectRuntime* rt);
        void execute();
        void setScript(ScriptRuntime* rt);
    };
}
