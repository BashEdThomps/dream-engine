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
    mState = luaL_newstate();
    luabind::open(mState);
    luaopen_base(mState);
    luabind::set_pcall_callback(&errorHandler);
    luaL_dostring(mState, mScriptLoaderCode.c_str());
    bindDreamEngine();
    bindAssetManager();
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
      LuaScriptInstance *luaScript = scriptIt->second;
      if (!luaScript->isLoaded()) {
        cout << "LuaComponent: Loading Lua script from " << luaScript->getAbsolutePath()
             << endl;
        if (!loadScript(luaScript)) {
          return false;
        }
        cout << "LuaComponent: Loaded " << luaScript->getUUID() << " Successfully" << endl;
        luaScript->setLoaded(true);
      }
    }
    return true;
  }

  bool LuaComponent::loadScript(LuaScriptInstance* scriptInstance) {
    string id = scriptInstance->getUUID();
    cout << "LuaComponent: loadScript called for " << id << endl;
    luabind::object newtable = luabind::newtable(mState);
    try {
      string path = scriptInstance->getAbsolutePath();
      luabind::call_function<void>(mState, "scriptloader", newtable, path.c_str());
      luabind::object reg = luabind::registry(mState);
      reg[id] = newtable;
    } catch (luabind::error e) {
      cerr << "LuaComponent: loadScript exception:" << endl
           << "\t" << e.what() << endl;
      return false;
    }
    return true;
  }

  void LuaComponent::bindDreamEngine() {
    luabind::module(mState) [
        luabind::class_<DreamEngine>("DreamEngine")
            .def("getAssetManager",&DreamEngine::getAssetManager)
            .def("getActiveScene",&DreamEngine::getActiveScene)
            .def("getTime",&DreamEngine::getTime)
            .scope [
               luabind::def("getInstance",&DreamEngine::getInstance)
            ]
    ];
  }

  void LuaComponent::bindAssetManager() {
    luabind::module(mState) [
        luabind::class_<AssetManager>("AssetManager")
        .def(luabind::constructor<>())
        .def(
          "createAssetInstancesForSceneObject",
          &AssetManager::createAssetInstancesForSceneObject
        )
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
        .def(luabind::constructor<AssetManager*>())
    ];
  }

  void LuaComponent::bindScene() {
    luabind::module(mState) [
        luabind::class_<Scene>("Scene")

        .def(luabind::constructor<>())

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

        .def("getUuid",&SceneObject::getUUID)
        .def("setUuid",&SceneObject::setUUID)

        .def("getName",&SceneObject::getName)
        .def("setName",&SceneObject::setName)

        .def("showStatus",&SceneObject::showStatus)

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
        .def_readwrite("type", &SDL_Event::type)
        .def_readwrite("key", &SDL_Event::key)
        .enum_("EventType") [
            luabind::value("KEYUP",SDL_EventType::SDL_KEYUP),
            luabind::value("KEYDOWN",SDL_EventType::SDL_KEYDOWN)
        ]
        .enum_("Key") [
            luabind::value("KEY_SPACE",SDLK_SPACE),
            luabind::value("KEY_LEFT",SDLK_LEFT),
            luabind::value("KEY_RIGHT",SDLK_RIGHT),
            luabind::value("KEY_UP",SDLK_UP),
            luabind::value("KEY_DOWN",SDLK_DOWN)

        ],
        // SDL_KeyboardEvent
        luabind::class_<SDL_KeyboardEvent>("KeyboardEvent")
            .def_readwrite("keysym",&SDL_KeyboardEvent::keysym),
        // SDL_Keysym
        luabind::class_<SDL_Keysym>("Keysym")
            .def_readwrite("sym",&SDL_Keysym::sym),
        // SDL_Keycode
        luabind::class_<SDL_Keycode>("Keycode")
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
          .def("getStatus",&AudioInstance::getStatus)
          .def("setStatus",&AudioInstance::setStatus),

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
    string id = script->getUUID();
    luabind::object reg = luabind::registry(mState);
    luabind::object table = reg[id];
    try {
      luabind::object funq = table[LUA_SCRIPT_UPDATE_FUNCTION];
      luabind::call_function<void>(funq,sceneObject);
    } catch (luabind::error e) {
      cerr << "LuaComponent: update exception:" << endl
           << "\t" << e.what() << endl;
      return false;
    }
    return true;
  }

  bool LuaComponent::executeScriptKeyHandler(SceneObject* sceneObject, LuaScriptInstance* script) {
    string id = script->getUUID();
    luabind::object reg = luabind::registry(mState);
    luabind::object table = reg[id];
    try {
      luabind::object funq = table[LUA_SCRIPT_HANDLE_INPUT_FUNCTION];
      luabind::call_function<void>(funq,sceneObject,mEvent);
    } catch (luabind::error &e) {
      cerr << "LuaComponent: handleInput exception:" << endl
           << "\t" << e.what() << endl;
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
  cout << str.str() << endl;
  // log the callstack
  std::string traceback = luabind::call_function<std::string>( luabind::globals(L)["debug"]["traceback"] );
  traceback = std::string( "Lua - Traceback: " ) + traceback;
  cout << traceback.c_str() << endl;
  // return unmodified error object
  return 1;
}
