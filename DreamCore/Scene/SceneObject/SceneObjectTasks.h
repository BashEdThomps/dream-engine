#pragma once

#include "../../TaskManager/Task.h"

namespace Dream
{
    class SceneObjectRuntime;
    class LifetimeUpdateTask : public Task
    {
    protected:
        SceneObjectRuntime* mSceneObjectRuntime;
    public:
        LifetimeUpdateTask(SceneObjectRuntime* rt);
        void execute();
    };
}
