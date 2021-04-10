#pragma once

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include "ScriptRuntime.h"
#include "Components/Component.h"
#include <memory>

using std::reference_wrapper;

namespace octronic::dream
{
  class SceneRuntime;
  class EntityRuntime;
  class Event;
  class ScriptPrintListener;

  class ScriptComponent : public Component
  {
  public: // Static
    static vector<reference_wrapper<ScriptPrintListener>> PrintListeners;
    static void AddPrintListener(ScriptPrintListener& listener);

  public:
    ScriptComponent(ProjectRuntime& runtime);
    ScriptComponent(ScriptComponent&&) = default;
    ScriptComponent& operator=(ScriptComponent&&) = default;
    ~ScriptComponent();

    bool init() override;

    bool executeScriptOnUpdate(ScriptRuntime& script, EntityRuntime& entity);
    bool executeScriptOnInit(ScriptRuntime& script, EntityRuntime& entity);
    bool executeScriptOnEvent(ScriptRuntime& script, EntityRuntime& entity);
    bool executeScriptOnInput(ScriptRuntime& script, SceneRuntime& sr);
    bool registerInputScript(ScriptRuntime& script);
    bool removeInputScript(UuidType script);
    bool createEntityState(ScriptRuntime& script, EntityRuntime& entity);
    bool removeEntityState(UuidType uuid);

    void pushTasks() override;
    lua_State* getLuaState() const;


  private:
    const static string LUA_COMPONENTS_TBL;
    const static string LUA_ON_INIT_FUNCTION;
    const static string LUA_ON_UPDATE_FUNCTION;
    const static string LUA_ON_INPUT_FUNCTION;
    const static string LUA_ON_EVENT_FUNCTION;

    // API Exposure Methods ================================================

    void debugRegisteringClass(const string& classname);
    void exposeAPI();

    // Base Classes
    void exposeRuntime();
    void exposeDefinition();
    void exposeAssetRuntime();
    void exposeSharedAssetRuntime();
    void exposeDiscreteAssetRuntime();

    // Runtimes
    void exposeAnimationRuntime();
    void exposeAudioRuntime();
    void exposeEntityRuntime();
    void exposeModelRuntime();
    void exposePathRuntime();
    void exposePhysicsRuntime();
    void exposeProjectRuntime();
    void exposeSceneRuntime();
    void exposeScriptRuntime();
    void exposeShaderRuntime();

    // Components
    void exposeAudioComponent();
    void exposeInputComponent();
    void exposeGraphicsComponent();
    void exposePhysicsComponent();
    void exposeWindowComponent();

    // Misc
    void exposeCamera();
    void exposeEvent();
    void exposeTime();
    void exposeTransform();
    void exposeGLM();
  private:
    lua_State* mLuaState;
  };
}
