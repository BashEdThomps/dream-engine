#include "ScriptRuntimeState.h"

#include "ScriptRuntime.h"
#include "../../Scene/SceneRuntime.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"

namespace Dream
{
    ScriptRuntimeState::ScriptRuntimeState
    (ScriptRuntime* sc, SceneObjectRuntime* rt)
        : script(sc),
          runtime(rt),
          initialised(false),
          error(false)
    {
        rt->setScriptRuntimeState(this);
    }
}
