/*
 * LuaComponent
 *
 * Copyright 2016 Octronic. All rights reserved.
 *
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

#include <sstream>
#include "LuaComponent.h"
#include "SDL2/SDL_keycode.h"
#include <luabind/adopt_policy.hpp>

namespace Dream {

  LuaComponent::LuaComponent() {
    mState = nullptr;
  }

  LuaComponent::~LuaComponent() {
    if (mState != nullptr) {
      lua_close(mState);
    }
  }

  bool LuaComponent::init() {
    if (DEBUG) {
      cout << "LuaComponent: Initialising LuaComponent" << endl;
    }
    mState = luaL_newstate();
    luabind::open(mState);
    luaopen_base(mState);
    luabind::set_pcall_callback(&errorHandler);
    luaL_dostring(mState, mScriptLoaderCode.c_str());
    bindDreamEngine();
    bindComponents();
    bindAssetClasses();
    bindProject();
    bindSceneObject();
    bindScene();
    bindTransform3D();
    bindSDL();
    return mState != nullptr;
  }

  bool LuaComponent::loadScriptsFromMap() {
    map<SceneObject*,LuaScriptInstance*>::iterator scriptIt;
    for (scriptIt=mScriptMap->begin(); scriptIt != mScriptMap->end(); scriptIt++) {

      SceneObject *sceneObject = scriptIt->first;
      LuaScriptInstance *luaScript = scriptIt->second;

      if (luaScript == nullptr) {
        if (DEBUG) {
          cerr << "LuaComponent: Load Failed, LuaScriptInstance is NULL" << endl;
        }
        continue;
      }

      if (sceneObject == nullptr) {

        if (DEBUG) {
          cerr << "LuaComponent: Load Failed, SceneObject is NULL" << endl;
        }
        continue;
      }

      if (luaScript->getLoadedFlag()) {
        continue;
      }

      if (sceneObject->getDeleteFlag()) {

        if (DEBUG) {
          cout << "LuaComponent: SceneObject marked for deletion, not loading script." << endl;
        }
        continue;
      }

      if (DEBUG) {
        cout << "LuaComponent: loading script '" << luaScript->getName() << "' for '" << sceneObject->getName() << "'" << endl;
        cout << "LuaComponent: Loading Lua script from " << luaScript->getAbsolutePath()
             << endl << flush;
      }

      if (!loadScript(sceneObject, luaScript)) {
        return false;
      }

      if (DEBUG) {
        cout << "LuaComponent: Loaded " << sceneObject->getUuid() << " Successfully" << endl;
      }
      luaScript->setLoadedFlag(true);

    }

    return true;
  }

  bool LuaComponent::loadScript(SceneObject* sceneObject, LuaScriptInstance* scriptInstance) {
    string id = sceneObject->getUuid();

    if (DEBUG) {
      cout << "LuaComponent: loadScript called for " << id << endl;
    }
    luabind::object newtable = luabind::newtable(mState);
    try {
      string path = scriptInstance->getAbsolutePath();
      luabind::call_function<void>(mState, "scriptloader", newtable, path.c_str());
      luabind::object reg = luabind::registry(mState);
      reg[id] = newtable;
    } catch (luabind::error e) {

      if (DEBUG) {
        cerr << "LuaComponent: loadScript exception:" << endl
             << "\t" << e.what() << endl;
      }
      return false;
    }
    return true;
  }

  void LuaComponent::bindDreamEngine() {
    luabind::module(mState) [
        luabind::class_<DreamEngine>("DreamEngine")
        .def("getActiveScene",&DreamEngine::getActiveScene)
        .def("getTime",&DreamEngine::getTime)
        .def("loadSceneByUuid",&DreamEngine::loadSceneByUuid)
        .scope [
        luabind::def("getInstance",&DreamEngine::getInstance)
        ]
        ];
  }

  void LuaComponent::bindComponents() {
    bindAnimationComponent();
    bindAudioComponent();
    bindGraphicsComponent();
    bindPhysicsComponent();
  }

  void LuaComponent::bindAnimationComponent() {
    // TODO
  }

  void LuaComponent::bindAudioComponent() {
    // TODO
  }

  void LuaComponent::bindGraphicsComponent() {
    // TODO
  }

  void LuaComponent::bindPhysicsComponent() {
    // TODO
  }

  void LuaComponent::bindProject() {
    luabind::module(mState) [
        luabind::class_<Project>("Project")
    ];
  }

  void LuaComponent::bindScene() {
    luabind::module(mState) [
        luabind::class_<Scene>("Scene")

        .def("getCameraMovementSpeed",&Scene::getCameraMovementSpeed)
        .def("setCameraMovementSpeed",&Scene::setCameraMovementSpeed)

        .def("getSceneObjectByUuid",&Scene::getSceneObjectByUuid)
        .def("getSceneObjectByName",&Scene::getSceneObjectByName)
        ];
  }

  void LuaComponent::bindSceneObject() {
    luabind::module(mState) [
        luabind::class_<SceneObject>("SceneObject")
        .def(luabind::constructor<>())

        .def("getUuid",&SceneObject::getUuid)
        .def("setUuid",&SceneObject::setUuid)

        .def("getName",&SceneObject::getName)
        .def("setName",&SceneObject::setName)

        .def("showStatus",&SceneObject::showStatus)

        .def("getChildByUuid",&SceneObject::getChildByUuid)

        .def("getTransform",&SceneObject::getTransform)
        .def("setTransform",&SceneObject::setTransform)

        .def("getParent",&SceneObject::getParent)
        .def("setParent",&SceneObject::setParent)
        .def("addChild",&SceneObject::addChild,luabind::adopt(_2))

        .def("copyTransform",&SceneObject::copyTransform)

        .def("addAssetDefUuidToLoad",&SceneObject::addAssetDefUuidToLoad)

        .def("getAnimationInstance",&SceneObject::getAnimationInstance)
        .def("setAnimationInstance",&SceneObject::setAnimationInstance)

        .def("getAudioInstance",&SceneObject::getAudioInstance)
        .def("setAudioInstance",&SceneObject::setAudioInstance)

        .def("getSpriteInstance",&SceneObject::getSpriteInstance)
        .def("setSpriteInstance",&SceneObject::setSpriteInstance)

        .def("getModelInstance",&SceneObject::getModelInstance)
        .def("setModelInstance",&SceneObject::setModelInstance)

        .def("getLightInstance",&SceneObject::getLightInstance)
        .def("setLightInstance",&SceneObject::setLightInstance)

        .def("getFontInstance",&SceneObject::getFontInstance)
        .def("setFontInstance",&SceneObject::setFontInstance)

        .def("getPhysicsObjectInstance",&SceneObject::getPhysicsObjectInstance)
        .def("setPhysicsObjectInstance",&SceneObject::setPhysicsObjectInstance)

        .def("getDeleteFlag",&SceneObject::getDeleteFlag)
        .def("setDeleteFlag",&SceneObject::setDeleteFlag)
        ];
  }

  void LuaComponent::bindTransform3D() {
    luabind::module(mState) [
        luabind::class_<Transform3D>("Transform3D")
        .def(luabind::constructor<>())
        // Translation ===========================================================
        .def("getTransformType",&Transform3D::getTransformType)
        .def("getTranslationX",&Transform3D::getTranslationX)
        .def("getTranslationY",&Transform3D::getTranslationY)
        .def("getTranslationZ",&Transform3D::getTranslationZ)
        .def("setTranslationX",&Transform3D::setTranslationX)
        .def("setTranslationY",&Transform3D::setTranslationY)
        .def("setTranslationZ",&Transform3D::setTranslationZ)
        .def("translateByX",&Transform3D::translateByX)
        .def("translateByY",&Transform3D::translateByY)
        .def("translateByZ",&Transform3D::translateByZ)
        // Rotation =============================================================
        .def("getRotationX",&Transform3D::getRotationX)
        .def("getRotationY",&Transform3D::getRotationY)
        .def("getRotationZ",&Transform3D::getRotationZ)
        .def("setRotationX",&Transform3D::setRotationX)
        .def("setRotationY",&Transform3D::setRotationY)
        .def("setRotationZ",&Transform3D::setRotationZ)
        .def("rotateByX",&Transform3D::rotateByX)
        .def("rotateByY",&Transform3D::rotateByY)
        .def("rotateByZ",&Transform3D::rotateByZ)
        // Scale ================================================================
        .def("getScaleX",&Transform3D::getScaleX)
        .def("getScaleY",&Transform3D::getScaleY)
        .def("getScaleZ",&Transform3D::getScaleZ)
        .def("setScaleX",&Transform3D::setScaleX)
        .def("setScaleY",&Transform3D::setScaleY)
        .def("setScaleZ",&Transform3D::setScaleZ)
        .def("scaleByX",&Transform3D::scaleByX)
        .def("scaleByY",&Transform3D::scaleByY)
        .def("scaleByZ",&Transform3D::scaleByZ)

        ];
  }

  void LuaComponent::bindTime() {
    luabind::module(mState) [
        luabind::class_<Time>("Time")
        .def("getCurrentTime",&Time::getCurrentTime)
        .def("getLastTime",&Time::getLastTime)
        .def("getTimeDelta",&Time::getTimeDelta)
        ];
  }

  void LuaComponent::bindSDL() {
    bindSDL_Event();
  }

  void LuaComponent::bindSDL_Event() {
    luabind::module(mState) [
        // SDL_Event
        luabind::class_<SDL_Event>("Event")
            .def_readonly("type", &SDL_Event::type)
            // Keyboard
            .def_readonly("key", &SDL_Event::key)
            // Controller
            .def_readonly("cbutton", &SDL_Event::cbutton)
            .def_readonly("caxis", &SDL_Event::caxis)
            // Joystick
            .def_readonly("jbutton", &SDL_Event::jbutton)
            .def_readonly("jhat", &SDL_Event::jhat)
            .def_readonly("jaxis", &SDL_Event::jaxis)
       // SDL_EventType
        .enum_("EventType") [
            luabind::value("KEYUP",SDL_EventType::SDL_KEYUP),
            luabind::value("KEYDOWN",SDL_EventType::SDL_KEYDOWN),

            luabind::value("JOY_BUTTONDOWN",SDL_JOYBUTTONDOWN),
            luabind::value("JOY_BUTTONUP",SDL_JOYBUTTONUP),
            luabind::value("JOY_HATMOTION",SDL_JOYHATMOTION),
            luabind::value("JOY_AXISMOTION",SDL_JOYAXISMOTION),

            luabind::value("CONTROLLER_BUTTONDOWN",SDL_CONTROLLERBUTTONDOWN),
            luabind::value("CONTROLLER_BUTTONUP",SDL_CONTROLLERBUTTONUP)
        ]
        // SDLK_* Definitions
        .enum_("Key") [
            luabind::value("KEY_RETURN",SDLK_RETURN),
            luabind::value("KEY_SPACE",SDLK_SPACE),
            luabind::value("KEY_LEFT",SDLK_LEFT),
            luabind::value("KEY_RIGHT",SDLK_RIGHT),
            luabind::value("KEY_UP",SDLK_UP),
            luabind::value("KEY_DOWN",SDLK_DOWN)
        ],
        // SDL_ControllerButtonEvent
        luabind::class_<SDL_ControllerButtonEvent>("ControllerButtonEvent")
            .def_readonly("type",&SDL_ControllerButtonEvent::type)
            .def_readonly("timestamp",&SDL_ControllerButtonEvent::timestamp)
            .def_readonly("which",&SDL_ControllerButtonEvent::which)
            .def_readonly("button",&SDL_ControllerButtonEvent::button)
            .def_readonly("state",&SDL_ControllerButtonEvent::state),
        // SDL_KeyboardEvent
        luabind::class_<SDL_KeyboardEvent>("KeyboardEvent")
            .def_readonly("keysym",&SDL_KeyboardEvent::keysym),
        // SDL_Keysym
        luabind::class_<SDL_Keysym>("Keysym")
            .def_readonly("sym",&SDL_Keysym::sym),
        // SDL_Keycode
        luabind::class_<SDL_Keycode>("Keycode"),
        // SDL_JoyButtonEvent
        luabind::class_<SDL_JoyButtonEvent>("JoyButtonEvent")
            .def_readonly("type",&SDL_JoyButtonEvent::type)
            .def_readonly("timestamp",&SDL_JoyButtonEvent::timestamp)
            .def_readonly("which",&SDL_JoyButtonEvent::which)
            .def_readonly("button",&SDL_JoyButtonEvent::button)
            .def_readonly("state",&SDL_JoyButtonEvent::state),
        // SDL_JoyHatEvent
        luabind::class_<SDL_JoyHatEvent>("JoyHatEvent")
            .def_readonly("type",&SDL_JoyHatEvent::type)
            .def_readonly("timestamp",&SDL_JoyHatEvent::timestamp)
            .def_readonly("which",&SDL_JoyHatEvent::which)
            .def_readonly("hat",&SDL_JoyHatEvent::hat)
            .def_readonly("value",&SDL_JoyHatEvent::value),
        // SDL_JoyAxisEvent
        luabind::class_<SDL_JoyAxisEvent>("JoyAxisEvent")
            .def_readonly("type",&SDL_JoyAxisEvent::type)
            .def_readonly("timestamp",&SDL_JoyAxisEvent::timestamp)
            .def_readonly("which",&SDL_JoyAxisEvent::which)
            .def_readonly("axis",&SDL_JoyAxisEvent::axis)
            .def_readonly("value",&SDL_JoyAxisEvent::value)
        ];
  }

  void LuaComponent::bindAssetClasses() {
    bindAudioInstance();
    bindFontInstance();
    //bindAnimationInstance();
    //bindAssimpModelInstance();
    //bindLightInstance();
    //bindShaderInstance();
    //bindSpriteInstance();
    //bindPhysicsObjectInstane();
  }

  void LuaComponent::bindFontInstance() {
    luabind::module(mState) [
        luabind::class_<FontInstance>("FontInstance")
        .def("setText",&FontInstance::setText)
        .def("getText",&FontInstance::getText)
        ];

  }

  void LuaComponent::bindAudioInstance() {
    luabind::module(mState) [
        luabind::class_<AudioInstance>("AudioInstance")
        .def("play",&AudioInstance::play)
        .def("pause",&AudioInstance::pause)
        .def("stop",&AudioInstance::stop)
        .def("getStatus",&AudioInstance::getStatus),

        luabind::class_<AudioStatus>("AudioStatus")
        .enum_("AudioStatus") [
        luabind::value("PLAYING", AudioStatus::PLAYING),
        luabind::value("PAUSED",  AudioStatus::PAUSED),
        luabind::value("STOPPED", AudioStatus::STOPPED)
        ]
        ];
  }

  void LuaComponent::setLuaScriptMap(map<SceneObject*,LuaScriptInstance*>* scriptMap) {
    mScriptMap = scriptMap;
  }

  void LuaComponent::stackDump() {
    cout << "LuaComponent: Stack Dump!" << endl;
    int i;
    int top = lua_gettop(mState);
    // repeat for each level
    for (i = 1; i <= top; i++) {
      int t = lua_type(mState, i);
      switch (t) {
        // strings
        case LUA_TSTRING:
          cout << lua_tostring(mState, i);
          break;
          // booleans
        case LUA_TBOOLEAN:
          cout << (lua_toboolean(mState, i) ? "true" : "false");
          break;
          // numbers
        case LUA_TNUMBER:
          cout << lua_tonumber(mState, i);
          break;
          // other values
        default:
          cout << lua_typename(mState, t);
          break;
      }
      // put a separator
      cout << "  ";
    }
    // end the listing
    cout << endl;
  }

  bool LuaComponent::update() {
    map<SceneObject*,LuaScriptInstance*>::iterator scriptIt;
    for (scriptIt=mScriptMap->begin(); scriptIt != mScriptMap->end(); scriptIt++) {
      SceneObject* key = scriptIt->first;
      if (key->getDeleteFlag()) {
        continue;
      }
      LuaScriptInstance *value = scriptIt->second;
      if (!executeScriptUpdate(key,value)) {
        return false;
      }
      if (key->hasFocus()) {
        if (!executeScriptKeyHandler(key,value)) {
          return false;
        }
      }
    }
    return true;
  }

  bool LuaComponent::executeScriptUpdate(SceneObject* sceneObject, LuaScriptInstance* script) {
    //string id = script->getUuid();
    string id = sceneObject->getUuid();
    luabind::object reg = luabind::registry(mState);
    luabind::object table = reg[id];
    try {
      luabind::object funq = table[LUA_SCRIPT_UPDATE_FUNCTION];
      luabind::call_function<void>(funq,sceneObject);
    } catch (luabind::error e) {

      if (DEBUG) {
        cerr << "LuaComponent: update exception:" << endl
             << "\t" << e.what() << endl;
      }
      return false;
    }
    return true;
  }

  bool LuaComponent::executeScriptKeyHandler(SceneObject* sceneObject, LuaScriptInstance* script) {
    //string id = script->getUuid();
    string id = sceneObject->getUuid();
    luabind::object reg = luabind::registry(mState);
    luabind::object table = reg[id];
    try {
      luabind::object funq = table[LUA_SCRIPT_HANDLE_INPUT_FUNCTION];
      luabind::call_function<void>(funq,sceneObject,mEvent);
    } catch (luabind::error &e) {

      if (DEBUG) {
        cerr << "LuaComponent: handleInput exception:" << endl
             << "\t" << e.what() << endl;
      }
      return false;
    }
    return true;
  }

  void LuaComponent::setSDL_Event(SDL_Event event) {
    mEvent = event;
  }

} // End of Dream

using namespace std;

int errorHandler(lua_State *L) {
  // log the error message
  luabind::object msg(luabind::from_stack( L, -1 ));
  std::ostringstream str;
  str << "Lua - RuntimeError: " << msg;

  if (DEBUG) {
    cout << str.str() << endl;
  }
  // log the callstack
  std::string traceback = luabind::call_function<std::string>( luabind::globals(L)["debug"]["traceback"] );
  traceback = std::string( "Lua - Traceback: " ) + traceback;

  if (DEBUG) {
    cout << traceback.c_str() << endl;
  }
  // return unmodified error object
  return 1;
}
