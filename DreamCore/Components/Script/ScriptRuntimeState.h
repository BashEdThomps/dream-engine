#pragma once

namespace Dream
{
    class ScriptRuntime;
    class SceneObjectRuntime;

    class ScriptRuntimeState
    {
    public:
        ScriptRuntimeState(ScriptRuntime* sc, SceneObjectRuntime* rt);

        ScriptRuntime* script = nullptr;
        SceneObjectRuntime* runtime = nullptr;
        bool initialised = false;
        bool error = false;
    };
}
