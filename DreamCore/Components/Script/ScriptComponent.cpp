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

#include "ScriptComponent.h"

#include "ScriptRuntime.h"
#include "ScriptDefinition.h"
#include "ScriptPrintListener.h"

#include "Components/Event.h"
#include "Math/Transform.h"
#include "Components/Time.h"
#include "Components/Path/PathRuntime.h"
#include "Components/Animation/AnimationRuntime.h"
#include "Components/Audio/AudioComponent.h"
#include "Components/Audio/AudioRuntime.h"
#include "Components/Audio/AudioStatus.h"
#include "Components/Graphics/Model/ModelRuntime.h"
#include "Components/Graphics/CameraRuntime.h"
#include "Components/Graphics/GraphicsComponent.h"
#include "Components/Graphics/Shader/ShaderRuntime.h"
#include "Components/Input/InputComponent.h"
#include "Components/Physics/PhysicsComponent.h"
#include "Components/Physics/PhysicsObjectRuntime.h"
#include "Components/Cache.h"
#include "Components/Window/WindowComponent.h"

#include "Entity/EntityRuntime.h"
#include "Scene/SceneRuntime.h"
#include "Project/ProjectDirectory.h"
#include "Project/ProjectRuntime.h"


extern "C"
{
#include <lstate.h>
}

#define SOL_CHECK_ARGUMENTS 1
#define SOL_ALL_SAFETIES_ON 1
#include <sol.h>
#include "glm/vec3.hpp"
#include <glm/gtc/matrix_transform.hpp>

using glm::vec3;
using std::stringstream;
using std::exception;
using std::cout;
using std::cerr;
using std::string;

// Static ======================================================================

static int
_octronic_dream_sol_print(lua_State* L)
{
  int nargs = lua_gettop(L);
  stringstream stream;
  for (int i=1; i <= nargs; ++i)
  {
    stream << lua_tostring(L, i);
  }

  string out = stream.str();
  for (auto& listener : octronic::dream::ScriptComponent::PrintListeners)
  {
    listener.get().onPrint(out);
  }
  return 0;
}

static const struct luaL_Reg printlib [] = {{"print", _octronic_dream_sol_print}, {nullptr, nullptr}};

int _octronic_dream_sol_exception_handler
(lua_State* L, sol::optional<const std::exception&> maybe_exception, sol::string_view description)
{
  LOG_ERROR("LUA: Exception handler called\n\n{}\n\n {}",maybe_exception->what(), description);
  return sol::stack::push(L, "Something exploded");
}

inline void
_octronic_dream_sol_panic(sol::optional<std::string> maybe_msg)
{
  LOG_ERROR("*** OH NO, PANIK: Lua is in a panic state and will now abort() the application ***");
  LOG_ERROR("Do you feel lucky? Lets see...");
  if (maybe_msg)
  {
    const std::string& msg = maybe_msg.value();
    LOG_ERROR("\tLucky: Error message = \"{}\"",msg);
  }
  else
  {
    LOG_ERROR("\tUnlucky: There was no error message... :/");
  }
  // When this function exits, Lua will exhibit default behavior and abort()
}

namespace octronic::dream // ===================================================
{
  const string ScriptComponent::LUA_ON_INIT_FUNCTION   = "onInit";
  const string ScriptComponent::LUA_ON_UPDATE_FUNCTION = "onUpdate";
  const string ScriptComponent::LUA_ON_INPUT_FUNCTION  = "onInput";
  const string ScriptComponent::LUA_ON_EVENT_FUNCTION  = "onEvent";
  const string ScriptComponent::LUA_COMPONENTS_TBL     = "Components";


  ScriptComponent::ScriptComponent
  (ProjectRuntime& runtime)
    : Component(runtime),
      mLuaState(nullptr)

  {
    LOG_TRACE( "ScriptComponent: Constructing Object" );
  }


  ScriptComponent::~ScriptComponent
  ()
  {
    LOG_TRACE("ScriptComponent: Destroying Object");
    if (mLuaState != nullptr)
    {
      lua_close(mLuaState);
      mLuaState = nullptr;
    }
  }

  bool
  ScriptComponent::init
  ()
  {
    LOG_DEBUG( "ScriptComponent: Initialising" );
    mLuaState = luaL_newstate();
    lua_atpanic(mLuaState, sol::c_call<decltype(&_octronic_dream_sol_panic), &_octronic_dream_sol_panic> );
    sol::state_view sView(mLuaState);
    sView.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::string);
    sView.set_exception_handler(_octronic_dream_sol_exception_handler);
    sol::table comps(mLuaState, sol::create);
    sView[LUA_COMPONENTS_TBL] = comps;

    // Register print callback

    lua_getglobal(mLuaState, "_G");
    luaL_setfuncs(mLuaState, printlib, 0);
    lua_pop(mLuaState, 1);

    LOG_DEBUG("ScriptComponent: Got a sol state");
    exposeAPI();
    return true;
  }

  // Function Execution =======================================================

  bool
  ScriptComponent::executeScriptOnUpdate
  (ScriptRuntime& script, EntityRuntime& entity)
  {
    LOG_DEBUG("ScriptComponent: Calling onUpdate for {}",entity.getNameAndUuidString());
    sol::state_view solStateView(ScriptComponent::mLuaState);
    sol::table entityTable = solStateView[entity.getUuid()];

    if (entityTable == sol::lua_nil)
    {
      return false;
    }

    sol::protected_function onUpdateFunction = entityTable[LUA_ON_UPDATE_FUNCTION];
    auto result = onUpdateFunction(entity);
    if (!result.valid())
    {
      // An error has occured
      sol::error err = result;
      string what = err.what();
      LOG_ERROR("ScriptComponent: {} Could not execute onUpdate in lua script:\n{}",
                entity.getNameAndUuidString(),
                what);
      entity.setScriptError(true);
      return false;
    }
    return true;
  }

  bool
  ScriptComponent::executeScriptOnInit
  (ScriptRuntime& script,
   EntityRuntime& entity)
  {
    if (entity.getScriptError())
    {
      LOG_ERROR("ScriptComponent: Cannot init, script for {} in error state.", entity.getNameAndUuidString());
      return true;
    }

    if (!entity.getScriptInitialised())
    {
      LOG_TRACE("ScriptComponent: Script {} has not been initialised for {}", script.getNameAndUuidString(), entity.getNameAndUuidString());
      return true;
    }

    LOG_DEBUG("ScriptComponent: Calling onInit in {} for {}",  script.getName(), entity.getName());

    sol::state_view solStateView(ScriptComponent::mLuaState);

    sol::table entityTable = solStateView[entity.getUuid()];

    if (entityTable == sol::lua_nil)
    {
      return false;
    }

    sol::protected_function onInitFunction = entityTable[LUA_ON_INIT_FUNCTION];

    auto initResult = onInitFunction(entity);

    if (!initResult.valid())
    {
      // An error has occured
      sol::error err = initResult;
      string what = err.what();
      LOG_ERROR("ScriptComponent: {}\nCould not execute onInit in lua script:\n{}",
                entity.getNameAndUuidString(), what);
      entity.setScriptError(true);
      return false;
    }
    entity.setScriptInitialised(true);
    return true;
  }

  bool
  ScriptComponent::executeScriptOnEvent
  (ScriptRuntime& script,
   EntityRuntime& entity)
  {
    if (entity.getScriptError())
    {
      LOG_ERROR( "ScriptComponent: Cannot execute {} in error state ",  script.getNameAndUuidString());
      entity.clearProcessedEvents();
      return true;
    }

    if (!entity.hasEvents())
    {
      return true;
    }


    LOG_DEBUG( "ScriptComponent: Calling onEvent for {}", entity.getNameAndUuidString());
    sol::state_view solStateView(ScriptComponent::mLuaState);

    sol::table entityTable = solStateView[entity.getUuid()];

    if (entityTable == sol::lua_nil)
    {
      return false;
    }

    sol::protected_function onEventFunction = entityTable[LUA_ON_EVENT_FUNCTION];

    for (auto e : entity.getEventQueue())
    {
      auto result = onEventFunction(entity,e);
      if (!result.valid())
      {
        // An error has occured
        sol::error err = result;
        string what = err.what();
        LOG_ERROR("ScriptComponent: {}:\nCould not execute onEvent in lua script:\n{}",
                  entity.getNameAndUuidString(), what);
        entity.setScriptError(true);
        return false;
      }
    }
    entity.clearProcessedEvents();
    return true;
  }

  bool
  ScriptComponent::executeScriptOnInput
  (ScriptRuntime& script,
   SceneRuntime& sr)
  {
    auto& inputComp = getProjectRuntime().getInputComponent();

    LOG_TRACE("ScriptComponent: Calling onInput function with {}",script.getUuid());
    sol::state_view solStateView(ScriptComponent::mLuaState);
    sol::table inputScriptTable = solStateView[script.getUuid()];

    if (inputScriptTable == sol::lua_nil)
    {
      return false;
    }

    sol::protected_function onInputFunction = inputScriptTable[LUA_ON_INPUT_FUNCTION];
    auto result = onInputFunction(inputComp, sr);
    if (!result.valid())
    {
      // An error has occured
      sol::error err = result;
      string what = err.what();
      LOG_ERROR("ScriptComponent: Could not execute onInput in lua script:\n{}",what);
      return false;
    }
    return true;
  }


  bool
  ScriptComponent::registerInputScript
  (ScriptRuntime& script)
  {
    LOG_TRACE("ScriptComponent: Registering Input Script");
    sol::state_view solStateView(ScriptComponent::mLuaState);
    sol::environment environment(ScriptComponent::mLuaState, sol::create, solStateView.globals());

    solStateView[script.getUuid()] = environment;

    auto exec_result = solStateView.safe_script(
          script.getSource(), environment,
          [](lua_State*, sol::protected_function_result pfr){
        return pfr;});

    // it did not work
    if(!exec_result.valid())
    {
      // An error has occured
      sol::error err = exec_result;
      string what = err.what();
      LOG_ERROR("ScriptComponent: Could not execute lua script:\n{}",what);
      return false;
    }

    LOG_DEBUG("ScriptComponent: Loaded Input Script Successfully");
    return true;
  }

  bool
  ScriptComponent::removeInputScript
  (UuidType script)
  {
    LOG_TRACE("ScriptComponent: Removing Input Script Table");
    sol::state_view solStateView(ScriptComponent::mLuaState);
    if (solStateView[script] != sol::lua_nil)
    {
      solStateView[script] = sol::lua_nil;
      LOG_DEBUG("ScriptComponent: Removed input script Successfully");
      return true;
    }
    LOG_ERROR("ScriptComponent: Failed to remove input script");
    return false;
  }

  bool
  ScriptComponent::createEntityState
  (ScriptRuntime& script,
   EntityRuntime& entity)
  {
    LOG_DEBUG("ScriptComponent: loadScript called for {}", entity.getNameAndUuidString() );
    LOG_DEBUG("ScriptComponent: calling scriptLoadFromString in lua for {}" , entity.getNameAndUuidString() );

    sol::state_view solStateView(ScriptComponent::mLuaState);

    // Create an environment for this entity runtime
    sol::environment environment(ScriptComponent::mLuaState, sol::create, solStateView.globals());
    solStateView[entity.getUuid()] = environment;

    auto exec_result = solStateView.safe_script(script.getSource(), environment,
                                                [](lua_State*, sol::protected_function_result pfr)
    {return pfr;});

    // it did not work
    if(!exec_result.valid())
    {
      // An error has occured
      sol::error err = exec_result;
      std::string what = err.what();
      LOG_ERROR("ScriptComponent: {} Error while executing lua script:\n{}", entity.getUuid(),what);
      entity.setScriptError(true);
      return false;
    }
    entity.setScriptInitialised(true);
    return true;
  }

  bool
  ScriptComponent::removeEntityState
  (UuidType uuid)
  {
    sol::state_view stateView(ScriptComponent::mLuaState);
    stateView[uuid] = sol::lua_nil;
    LOG_DEBUG( "ScriptComponent: Removed script lua table for {}" , uuid);
    return true;
  }

  // API Exposure Methods ======================================================

  void
  ScriptComponent::exposeProjectRuntime
  ()
  {
    debugRegisteringClass("ProjectRuntime");
    sol::state_view stateView(mLuaState);
    stateView.new_usertype<ProjectRuntime>("ProjectRuntime");
  }

  void
  ScriptComponent::exposeCamera
  ()
  {
    debugRegisteringClass("Camera");
    sol::state_view stateView(mLuaState);
    stateView.new_usertype<CameraRuntime>(
          "Camera",
          "setTransform",&CameraRuntime::setTransform,
          "getTransform",&CameraRuntime::getTransform);

    stateView.new_enum(
          "FrustumPlane",
          "Back",Frustum::Plane::PLANE_BACK,
          "Front",Frustum::Plane::PLANE_FRONT,
          "Right",Frustum::Plane::PLANE_RIGHT,
          "Left",Frustum::Plane::PLANE_LEFT,
          "Top",Frustum::Plane::PLANE_TOP,
          "Bottom",Frustum::Plane::PLANE_BOTTOM);

    stateView.new_enum(
          "FrustumTestResult",
          "Inside",Frustum::TestResult::TEST_INSIDE,
          "Outside",Frustum::TestResult::TEST_OUTSIDE,
          "Intersect",Frustum::TestResult::TEST_INTERSECT);
  }

  void
  ScriptComponent::exposePathRuntime
  ()
  {
    debugRegisteringClass("PathRuntime");
    sol::state_view stateView(mLuaState);
    stateView.new_usertype<PathRuntime>(
          "PathRuntime",
          "generate",&PathRuntime::generate,
          "getSplinePoints",&PathRuntime::getSplinePoints,
          "getSplinePoint",&PathRuntime::getSplinePoint,
          "getUStep",&PathRuntime::getUStep,
          "setUStep",&PathRuntime::setUStep,
          "stepPath",&PathRuntime::stepPath);
  }

  void
  ScriptComponent::exposeGraphicsComponent
  ()
  {
    debugRegisteringClass("GraphicsComponent");
    sol::state_view stateView(mLuaState);
    stateView.new_usertype<GraphicsComponent>("GraphicsComponent");
  }

  void
  ScriptComponent::exposeShaderRuntime
  ()
  {
    debugRegisteringClass("ShaderRuntime");
    sol::state_view stateView(mLuaState);
    stateView.new_usertype<ShaderRuntime>(
          "ShaderRuntime",
          "getUuid", &ShaderRuntime::getUuid);
  }

  void
  ScriptComponent::exposePhysicsComponent
  ()
  {
    debugRegisteringClass("PhysicsComponent");
    sol::state_view stateView(mLuaState);
    stateView.new_usertype<PhysicsComponent>(
          "PhysicsComponent");
  }

  void
  ScriptComponent::exposePhysicsObjectRuntime
  ()
  {
    debugRegisteringClass("PhysicsObjectRuntime");
    sol::state_view stateView(mLuaState);
    stateView.new_usertype<PhysicsObjectRuntime>(
          "PhysicsObjectRuntime",
          "getUuid", &PhysicsObjectRuntime::getUuid,
          "getMass", &PhysicsObjectRuntime::getMass,
          "setMass", &PhysicsObjectRuntime::setMass,
          "getLinearVelocity", &PhysicsObjectRuntime::getLinearVelocity,
          "setLinearVelocity", &PhysicsObjectRuntime::setLinearVelocity,
          "setLinearFactor", &PhysicsObjectRuntime::setLinearFactor,
          "setAngularFactor", &PhysicsObjectRuntime::setAngularFactor,
          "setAngularVelocity", &PhysicsObjectRuntime::setAngularVelocity,
          "getRestitution", &PhysicsObjectRuntime::getRestitution,
          "setRestitution", &PhysicsObjectRuntime::setRestitution,
          "getFriction", &PhysicsObjectRuntime::getFriction,
          "setFriction", &PhysicsObjectRuntime::setFriction,
          "clearForces",&PhysicsObjectRuntime::clearForces,
          "getCenterOfMassPosition",&PhysicsObjectRuntime::getCenterOfMassPosition,
          "setCenterOfMassTransformTx",&PhysicsObjectRuntime::setCenterOfMassTransformTx,
          "setCenterOfMassTransform3f",&PhysicsObjectRuntime::setCenterOfMassTransform3f,
          "setCenterOfMassTransform3fv",&PhysicsObjectRuntime::setCenterOfMassTransform3fv,
          "setCenterOfMassTransformMat4",&PhysicsObjectRuntime::setCenterOfMassTransformMat4,
          "setKinematicObject",&PhysicsObjectRuntime::setKinematic,
          "setGameModeCharacter",&PhysicsObjectRuntime::setCameraControllableCharacter);
  }

  void
  ScriptComponent::exposeEntityRuntime
  ()
  {
    debugRegisteringClass("EntityRuntime");
    sol::state_view stateView(mLuaState);
    stateView.new_usertype<EntityRuntime>(
          "EntityRuntime",
          sol::base_classes, sol::bases<Runtime>(),
          "getName",&EntityRuntime::getName,
          "getNameAndUuidString",&EntityRuntime::getNameAndUuidString,
          "getScene",&EntityRuntime::getSceneRuntime,
          "getChildByUuid",&EntityRuntime::getChildRuntimeByUuid,
          "getParentEntity",&EntityRuntime::getParentEntityRuntime,
          "setParentEntity",&EntityRuntime::setParentEntityRuntime,
          "getTransform",&EntityRuntime::getTransform,
          "setTransform",&EntityRuntime::setTransform,
          "getPathRuntime",&EntityRuntime::getPathRuntime,
          "getAnimationRuntime",&EntityRuntime::getAnimationRuntime,
          "getAudioRuntime",&EntityRuntime::getAudioRuntime,
          "getModelRuntime",&EntityRuntime::getModelRuntime,
          "getPhysicsObjectRuntime",&EntityRuntime::getPhysicsObjectRuntime,
          "hasPathRuntime",&EntityRuntime::hasPathRuntime,
          "hasAudioRuntime",&EntityRuntime::hasAudioRuntime,
          "hasModelRuntime",&EntityRuntime::hasModelRuntime,
          "hasPhysicsObjectRuntime",&EntityRuntime::hasPhysicsObjectRuntime,
          "getDeleted",&EntityRuntime::getDeleted,
          "setDeleted",&EntityRuntime::setDeleted,
          "addEvent",&EntityRuntime::addEvent,
          "containedInFrustum",&EntityRuntime::containedInFrustum);
  }

  void
  ScriptComponent::exposeTransform
  ()
  {
    debugRegisteringClass("Transform");
    sol::state_view stateView(mLuaState);
    stateView.new_usertype<Transform>(
          "Transform",
          "translate",&Transform::translate,
          "getTranslation",&Transform::getTranslation,
          "setTranslation",&Transform::setTranslation,
          "getPitch",&Transform::getPitch,
          "setPitch",&Transform::setPitch,
          "getYaw",&Transform::getYaw,
          "setYaw",&Transform::setYaw,
          "getRoll",&Transform::getRoll,
          "setRoll",&Transform::setRoll,
          "getScale",&Transform::getScale,
          "setScale",&Transform::setScale);
  }

  void
  ScriptComponent::exposeTime
  ()
  {
    debugRegisteringClass("Time");
    sol::state_view stateView(mLuaState);
    stateView.new_usertype<Time>(
          "Time",
          "getCurrentFrameTime",&Time::getCurrentFrameTime,
          "getLastFrameTime",&Time::getLastFrameTime,
          "getFrameTimeDelta",&Time::getFrameTimeDelta,
          "perSecond",&Time::perSecond);

    auto& time = getProjectRuntime().getTime();
    stateView["Time"] = std::ref(time);
  }

  void
  ScriptComponent::exposeModelRuntime
  ()
  {
    debugRegisteringClass("ModelRuntime");
    sol::state_view stateView(mLuaState);
    stateView.new_usertype<ModelRuntime>("ModelRuntime");
  }

  void
  ScriptComponent::exposeEvent
  ()
  {
    debugRegisteringClass("Event");
    sol::state_view stateView(mLuaState);
    stateView.new_usertype<Event>(
          "Event",
          "getAttribute",&Event::getAttribute,
          "setAttribute",&Event::setAttribute,
          "getProcessed",&Event::getProcessed,
          "setProcessed",&Event::setProcessed);
  }

  void
  ScriptComponent::exposeWindowComponent
  ()
  {
    debugRegisteringClass("WindowComponent");
    sol::state_view stateView(mLuaState);
    stateView.new_usertype<WindowComponent>(
          "WindowComponent",
          "getWidth",&WindowComponent::getWidth,
          "getHeight",&WindowComponent::getHeight);
  }

  void
  ScriptComponent::exposeInputComponent
  ()
  {
    debugRegisteringClass("InputComponent");
    sol::state_view stateView(mLuaState);
    stateView.new_usertype<InputComponent>(
          "InputComponent",
          sol::base_classes, sol::bases<Component>(),
          "getKeyboardState",&InputComponent::getKeyboardState,
          "getMouseState",&InputComponent::getMouseState,
          "getJoystickState",&InputComponent::getJoystickState,
          "getJoystickMapping",&InputComponent::getJoystickMapping,
          "getJoystickNavigation", &InputComponent::getJoystickNavigation,
          "getJoystickCount", &InputComponent::getJoystickCount);

    stateView.new_usertype<MouseState>(
          "MouseState",
          "getPosX", &MouseState::getPosX,
          "getPosY", &MouseState::getPosY,
          "getScrollX", &MouseState::getScrollX,
          "getScrollY", &MouseState::getScrollY,
          "isButtonPressed", &MouseState::isButtonPressed);

    stateView.new_usertype<JoystickMapping>(
          "JoystickMapping",
          "AnalogLeftXAxis",&JoystickMapping::AnalogLeftXAxis ,
          "AnalogLeftYAxis",&JoystickMapping::AnalogLeftYAxis ,
          "AnalogRightXAxis",&JoystickMapping::AnalogRightXAxis ,
          "AnalogRightYAxis",&JoystickMapping::AnalogRightYAxis ,
          "FaceButtonNorth",&JoystickMapping::FaceButtonNorth ,
          "FaceButtonEast",&JoystickMapping::FaceButtonEast ,
          "FaceButtonSouth",&JoystickMapping::FaceButtonSouth ,
          "FaceButtonWest",&JoystickMapping::FaceButtonWest ,
          "FaceButtonSelect",&JoystickMapping::FaceButtonSelect ,
          "FaceButtonStart",&JoystickMapping::FaceButtonStart ,
          "FaceButtonHome",&JoystickMapping::FaceButtonHome ,
          "ShoulderLeft",&JoystickMapping::ShoulderLeft ,
          "ShoulderRight",&JoystickMapping::ShoulderRight ,
          "TriggerLeftButton",&JoystickMapping::TriggerLeftButton ,
          "TriggerRightButton",&JoystickMapping::TriggerRightButton ,
          "AnalogLeftButton",&JoystickMapping::AnalogLeftButton ,
          "AnalogRightButton",&JoystickMapping::AnalogRightButton ,
          "DPadNorth",&JoystickMapping::DPadNorth,
          "DPadWest",&JoystickMapping::DPadWest,
          "DPadSouth",&JoystickMapping::DPadSouth,
          "DPadEast",&JoystickMapping::DPadEast);

    stateView.new_usertype<JoystickState>(
          "JoystickState",
          "getButtonData", &JoystickState::getButtonData,
          "setButtonData", &JoystickState::setButtonData,
          "getAxisData", &JoystickState::getAxisData,
          "setAxisData", &JoystickState::setAxisData,
          "getName", &JoystickState::getName,
          "getButtonCount", &JoystickState::getButtonCount,
          "setButtonCount", &JoystickState::setButtonCount,
          "getAxisCount", &JoystickState::getAxisCount,
          "setAxisCount", &JoystickState::setAxisCount,
          "getDeadZone", &JoystickState::getDeadZone);

    stateView.new_usertype<JoystickNavigation>(
          "JoystickNavigation",
          "update", &JoystickNavigation::update,
          "setHeading", &JoystickNavigation::setHeading,
          "getHeading", &JoystickNavigation::getHeading);

    stateView.new_usertype<KeyboardState>(
          "KeyboardState",
          "isKeyPressed", &KeyboardState::isKeyPressed);

    stateView.new_enum(
          "KeyboardMapping",
          "KEY_UNKNOWN",  -1,
          "KEY_SPACE",  32,
          "KEY_APOSTROPHE",  39,  /* ' */
          "KEY_COMMA",  44,  /* , */
          "KEY_MINUS",  45,  /* - */
          "KEY_PERIOD",  46,  /* . */
          "KEY_SLASH",  47,  /* / */
          "KEY_0",  48,
          "KEY_1",  49,
          "KEY_2",  50,
          "KEY_3",  51,
          "KEY_4",  52,
          "KEY_5",  53,
          "KEY_6",  54,
          "KEY_7",  55,
          "KEY_8",  56,
          "KEY_9",  57,
          "KEY_SEMICOLON",  59,  /* , */
          "KEY_EQUAL",  61,  /* ", */
          "KEY_A",  65,
          "KEY_B",  66,
          "KEY_C",  67,
          "KEY_D",  68,
          "KEY_E",  69,
          "KEY_F",  70,
          "KEY_G",  71,
          "KEY_H",  72,
          "KEY_I",  73,
          "KEY_J",  74,
          "KEY_K",  75,
          "KEY_L",  76,
          "KEY_M",  77,
          "KEY_N",  78,
          "KEY_O",  79,
          "KEY_P",  80,
          "KEY_Q",  81,
          "KEY_R",  82,
          "KEY_S",  83,
          "KEY_T",  84,
          "KEY_U",  85,
          "KEY_V",  86,
          "KEY_W",  87,
          "KEY_X",  88,
          "KEY_Y",  89,
          "KEY_Z",  90,
          "KEY_LEFT_BRACKET",  91,  /* [ */
          "KEY_BACKSLASH",  92,  /* \ */
          "KEY_RIGHT_BRACKET",  93,  /* ] */
          "KEY_GRAVE_ACCENT",  96,  /* ` */
          "KEY_WORLD_1",  161, /* non-US #1 */
          "KEY_WORLD_2",  162, /* non-US #2 */
          "KEY_ESCAPE",  256,
          "KEY_ENTER",  257,
          "KEY_TAB",  258,
          "KEY_BACKSPACE",  259,
          "KEY_INSERT",  260,
          "KEY_DELETE",  261,
          "KEY_RIGHT",  262,
          "KEY_LEFT",  263,
          "KEY_DOWN",  264,
          "KEY_UP",  265,
          "KEY_PAGE_UP",  266,
          "KEY_PAGE_DOWN",  267,
          "KEY_HOME",  268,
          "KEY_END",  269,
          "KEY_CAPS_LOCK",  280,
          "KEY_SCROLL_LOCK",  281,
          "KEY_NUM_LOCK",  282,
          "KEY_PRINT_SCREEN",  283,
          "KEY_PAUSE",  284,
          "KEY_F1",  290,
          "KEY_F2",  291,
          "KEY_F3",  292,
          "KEY_F4",  293,
          "KEY_F5",  294,
          "KEY_F6",  295,
          "KEY_F7",  296,
          "KEY_F8",  297,
          "KEY_F9",  298,
          "KEY_F10",  299,
          "KEY_F11",  300,
          "KEY_F12",  301,
          "KEY_F13",  302,
          "KEY_F14",  303,
          "KEY_F15",  304,
          "KEY_F16",  305,
          "KEY_F17",  306,
          "KEY_F18",  307,
          "KEY_F19",  308,
          "KEY_F20",  309,
          "KEY_F21",  310,
          "KEY_F22",  311,
          "KEY_F23",  312,
          "KEY_F24",  313,
          "KEY_F25",  314,
          "KEY_KP_0",  320,
          "KEY_KP_1",  321,
          "KEY_KP_2",  322,
          "KEY_KP_3",  323,
          "KEY_KP_4",  324,
          "KEY_KP_5",  325,
          "KEY_KP_6",  326,
          "KEY_KP_7",  327,
          "KEY_KP_8",  328,
          "KEY_KP_9",  329,
          "KEY_KP_DECIMAL",  330,
          "KEY_KP_DIVIDE",  331,
          "KEY_KP_MULTIPLY",  332,
          "KEY_KP_SUBTRACT",  333,
          "KEY_KP_ADD",  334,
          "KEY_KP_ENTER",  335,
          "KEY_KP_EQUAL",  336,
          "KEY_LEFT_SHIFT",  340,
          "KEY_LEFT_CONTROL",  341,
          "KEY_LEFT_ALT",  342,
          "KEY_LEFT_SUPER",  343,
          "KEY_RIGHT_SHIFT",  344,
          "KEY_RIGHT_CONTROL",  345,
          "KEY_RIGHT_ALT",  346,
          "KEY_RIGHT_SUPER",  347,
          "KEY_MENU",  348);
  }

  void
  ScriptComponent::exposeAudioComponent
  ()
  {
    debugRegisteringClass("AudioComponent");
    sol::state_view stateView(mLuaState);
    stateView.new_usertype<AudioComponent>("AudioComponent");
  }

  void
  ScriptComponent::exposeScriptRuntime
  ()
  {
    // TODO
  }

  void
  ScriptComponent::exposeAudioRuntime
  ()
  {
    debugRegisteringClass("AudioRuntime");
    sol::state_view stateView(mLuaState);
    stateView.new_usertype<AudioRuntime>(
          "AudioRuntime",
          "getState",&AudioRuntime::getState,
          "play",&AudioRuntime::play,
          "pause",&AudioRuntime::pause,
          "stop",&AudioRuntime::stop);

    stateView.new_enum(
          "AudioState",
          "PLAYING", AudioStatus::AUDIO_STATUS_PLAYING,
          "PAUSED",  AudioStatus::AUDIO_STATUS_PAUSED,
          "STOPPED", AudioStatus::AUDIO_STATUS_STOPPED);
  }

  void
  ScriptComponent::exposeSceneRuntime
  ()
  {
    debugRegisteringClass("SceneRuntime");
    sol::state_view stateView(mLuaState);
    stateView.new_usertype<SceneRuntime>(
          "SceneRuntime",
          sol::base_classes, sol::bases<Runtime>(),
          "getCamera",&SceneRuntime::getCameraRuntime,
          "getProjectRuntime", &SceneRuntime::getProjectRuntime,
          "getEntityRuntimeByUuid",&SceneRuntime::getEntityRuntimeByUuid);
  }

  void
  ScriptComponent::exposeGLM
  ()
  {
    debugRegisteringClass("GLM");
    sol::state_view stateView(mLuaState);

    stateView.new_usertype<vec3>(
          "vec3",
          "x", &vec3::x,
          "y", &vec3::y,
          "z", &vec3::z);

    stateView.new_usertype<glm::vec4>(
          "vec4",
          "x", &glm::vec4::x,
          "y", &glm::vec4::y,
          "z", &glm::vec4::z,
          "w", &glm::vec4::w);

    stateView.new_usertype<glm::quat>(
          "quat",
          sol::constructors<glm::quat(), glm::quat(float, float, float, float)>(),
          "w", &glm::quat::w,
          "x", &glm::quat::x,
          "y", &glm::quat::y,
          "z", &glm::quat::z);

    auto mat4MultiplicationOverloads = sol::overload(
          [](const glm::mat4& v1, const glm::mat4& v2) -> glm::mat4 { return v1*v2; },
    [](const glm::mat4& v1, float f) -> glm::mat4 { return v1*f; },
    [](float f, const glm::mat4& v1) -> glm::mat4 { return f*v1; });

    stateView.new_usertype<glm::mat4>(
          "mat4",
          sol::constructors<glm::mat4(), glm::mat4(float), glm::mat4(glm::mat4)>(),
          sol::meta_function::multiplication, mat4MultiplicationOverloads);

    stateView.set_function(
          "translate",
          [](const glm::mat4& m1, const glm::vec3& v1) -> glm::mat4{return glm::translate(m1,v1);});

    stateView.set_function(
          "translate3f",
          [](const glm::mat4& m1, float x, float y, float z) -> glm::mat4{return glm::translate(m1,vec3(x,y,z));});

    stateView.set_function(
          "rotate",
          [](const glm::mat4& m1, const float f1, const glm::vec3& v1) -> glm::mat4{return glm::rotate(m1,f1,v1);});

    stateView.set_function(
          "scale",
          [](const glm::mat4& m1, glm::vec3& v1) -> glm::mat4{return glm::scale(m1,v1);});

    stateView.set_function(
          "mat4_cast",
          [](const glm::quat& q1) -> glm::mat4{return glm::mat4_cast(q1);});

    stateView.set_function(
          "matrix_col",
          [](const glm::mat4& mtx, int col) -> glm::vec3{return  mtx[col];});
  }

  void
  ScriptComponent::exposeDefinition
  ()
  {
    debugRegisteringClass("Definition");
    sol::state_view stateView(mLuaState);
    stateView.new_usertype<Definition>("Definition");
  }

  void
  ScriptComponent::exposeAnimationRuntime
  ()
  {
    debugRegisteringClass("AnimationRuntime");
    sol::state_view stateView(mLuaState);
    stateView.new_usertype<AnimationRuntime>(
          "AnimationRuntime",
          "run",&AnimationRuntime::run,
          "pause",&AnimationRuntime::pause,
          "reset",&AnimationRuntime::reset);
  }

  void
  ScriptComponent::exposeRuntime()
  {
    debugRegisteringClass("Runtime");
    sol::state_view stateView(mLuaState);
    stateView.new_usertype<Runtime>("Runtime");
  }

  void
  ScriptComponent::exposeComponent()
  {
    debugRegisteringClass("Component");
    sol::state_view stateView(mLuaState);
    stateView.new_usertype<Component>("Component");
  }

  void
  ScriptComponent::exposeAssetRuntime()
  {
    debugRegisteringClass("AssetRuntime");
    sol::state_view stateView(mLuaState);
    stateView.new_usertype<AssetRuntime>(
          "AssetRuntime",
          sol::base_classes, sol::bases<Runtime>());
  }

  void
  ScriptComponent::exposeSharedAssetRuntime()
  {
    debugRegisteringClass("SharedAssetRuntime");
    sol::state_view stateView(mLuaState);
    stateView.new_usertype<SharedAssetRuntime>(
          "SharedAssetRuntime",
          sol::base_classes, sol::bases<AssetRuntime>());
  }

  void
  ScriptComponent::exposeDiscreteAssetRuntime()
  {
    debugRegisteringClass("DiscreteAssetRuntime");
    sol::state_view stateView(mLuaState);
    stateView.new_usertype<DiscreteAssetRuntime>(
          "DiscreteAssetRuntime",
          sol::base_classes, sol::bases<AssetRuntime>());
  }

  void
  ScriptComponent::debugRegisteringClass
  (const string& className)
  {
    LOG_DEBUG( "ScriptComponent: Registering Class {}",  className );
  }

  void
  ScriptComponent::exposeAPI
  ()
  {
    // Base Classes
    exposeRuntime();
    exposeComponent();
    exposeDefinition();
    exposeAssetRuntime();
    exposeSharedAssetRuntime();
    exposeDiscreteAssetRuntime();

    // Runtimes
    exposeAnimationRuntime();
    exposeAudioRuntime();
    exposeEntityRuntime();
    exposeModelRuntime();
    exposePathRuntime();
    exposePhysicsObjectRuntime();
    exposeProjectRuntime();
    exposeSceneRuntime();
    exposeScriptRuntime();
    exposeShaderRuntime();

    // Components
    exposeAudioComponent();
    exposeInputComponent();
    exposeGraphicsComponent();
    exposePhysicsComponent();
    exposeWindowComponent();

    // Misc
    exposeCamera();
    exposeEvent();
    exposeTime();
    exposeTransform();
    exposeGLM();
  }


  lua_State*
  ScriptComponent::getLuaState
  ()
  const
  {
    return mLuaState;
  }

  void ScriptComponent::pushTasks()
  {
    auto& scriptCache = getProjectRuntime().getScriptCache();
    for (auto& scriptRuntime : scriptCache.getRuntimeVector())
    {
      scriptRuntime.pushTasks();
    }
  }

  // Static ==================================================================

  vector<reference_wrapper<ScriptPrintListener>> ScriptComponent::PrintListeners;

  void
  ScriptComponent::AddPrintListener
  (ScriptPrintListener& listener)
  {
    PrintListeners.push_back(listener);
  }
}
