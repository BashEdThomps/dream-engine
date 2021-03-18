/*
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */

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

using std::shared_ptr;

namespace octronic::dream
{
  class SceneRuntime;
  class EntityRuntime;
  class Event;

  class ScriptPrintListener
  {
  public:
    virtual ~ScriptPrintListener();
    virtual void onPrint(const string&) = 0;
  };

  class ScriptComponent : public Component
  {
  public:
    static vector<shared_ptr<ScriptPrintListener>> PrintListeners;
    static void AddPrintListener(const shared_ptr<ScriptPrintListener>& listener);
    static shared_ptr<lua_State> LuaState;

    ScriptComponent(const weak_ptr<ProjectRuntime>& runtime);
    ~ScriptComponent() override;

    bool init() override;

    bool executeScriptOnUpdate(const weak_ptr<ScriptRuntime>& script, const weak_ptr<EntityRuntime>& entity);
    bool executeScriptOnInit(const weak_ptr<ScriptRuntime>& script, const weak_ptr<EntityRuntime>& entity);
    bool executeScriptOnEvent(const weak_ptr<ScriptRuntime>& script, const weak_ptr<EntityRuntime>& entity);
    bool executeScriptOnInput(const weak_ptr<ScriptRuntime>& script, const weak_ptr<SceneRuntime>& sr);
    bool registerInputScript(const weak_ptr<ScriptRuntime>& script);
    bool removeInputScript(UuidType script);
    bool createEntityState(const weak_ptr<ScriptRuntime>& script, const weak_ptr<EntityRuntime>& entity);
    bool removeEntityState(UuidType uuid);

    void pushTasks() override;


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
    void exposeComponent();
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
    void exposePhysicsObjectRuntime();
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
    void exposeProjectDirectory();
    void exposeCamera();
    void exposeEvent();
    void exposeTime();
    void exposeTransform();
    void exposeGLM();
  };
}
