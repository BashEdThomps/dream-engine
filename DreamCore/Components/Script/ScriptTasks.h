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

    class ScriptExecuteOnInitTask : public Task
    {
    public:
        void execute();
    };

    class ScriptExecuteOnUpdateTask : public Task
    {
    public:
        void execute();
    };

    class ScriptExecuteOnEventTask : public Task
    {
    public:
        void execute();
    };

    class ScriptExecuteOnNanoVGTask : public GraphicsComponentTask
    {
    public:
        void execute();
    };
}
