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
    bindAssetManager();
    bindComponents();
    bindAssetClasses();
    bindProject();
    bindSceneObject();
    bindScene();
    bindTransform3D();
    bindSDL();
    return loadScriptsFromMap();
  }

  bool LuaComponent::loadScriptsFromMap() {
    map<SceneObject*,LuaScriptInstance*>::iterator scriptIt;
    for (scriptIt=mScriptMap->begin(); scriptIt != mScriptMap->end(); scriptIt++) {
      LuaScriptInstance *value = scriptIt->second;
      cout << "LuaComponent: Loading Lua script from " << value->getAbsolutePath()
           << endl;
      if (!loadScript(value)) {
        return false;
      }
      cout << "LuaComponent: Loaded " << value->getUUID()
           << " Successfully" << endl;
    }
    return true;
  }

  bool LuaComponent::loadScript(LuaScriptInstance* scriptInstance) {
    string id = scriptInstance->getUUID();
    luaL_dostring(mState, mScriptLoaderCode.c_str());
    luabind::object newtable = luabind::newtable(mState);
    try {
      luabind::call_function<void>(
            mState, "scriptloader", newtable,
            scriptInstance->getAbsolutePath().c_str()
            );
      luabind::object reg = luabind::registry(mState);
      reg[id] = newtable;
    } catch (luabind::error e){
      cerr << "LuaComponent: loadScript exception:" << endl
           << "\t" << e.what() << endl;
      return false;
    }
    return true;
  }

  void LuaComponent::bindAssetManager() {
    luabind::module(mState) [
        luabind::class_<AssetManager>("AssetManager")
        .def(luabind::constructor<>())
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
    ];
  }

  void LuaComponent::bindSceneObject() {
    luabind::module(mState) [
        luabind::class_<SceneObject>("SceneObject")
        .def(luabind::constructor<>())
        .def("getUuid",&SceneObject::getUUID)
        .def("getTransform",&SceneObject::getTransform)
    ];
  }

  void LuaComponent::bindTransform3D() {
    luabind::module(mState) [
        luabind::class_<Transform3D>("Transform3D")
        .def(luabind::constructor<>())
        .def("getTransformType",&Transform3D::getTransformType)

        .def("getTranslationX",&Transform3D::getTranslationX)
        .def("getTranslationY",&Transform3D::getTranslationY)
        .def("getTranslationZ",&Transform3D::getTranslationZ)
        .def("setTranslationX",&Transform3D::setTranslationX)
        .def("setTranslationY",&Transform3D::setTranslationY)
        .def("setTranslationZ",&Transform3D::setTranslationZ)

        .def("getRotationX",&Transform3D::getRotationX)
        .def("getRotationY",&Transform3D::getRotationY)
        .def("getRotationZ",&Transform3D::getRotationZ)
        .def("setRotationX",&Transform3D::setRotationX)
        .def("setRotationY",&Transform3D::setRotationY)
        .def("setRotationZ",&Transform3D::setRotationZ)

        .def("getScaleX",&Transform3D::getScaleX)
        .def("getScaleY",&Transform3D::getScaleY)
        .def("getScaleZ",&Transform3D::getScaleZ)
        .def("setScaleX",&Transform3D::setScaleX)
        .def("setScaleY",&Transform3D::setScaleY)
        .def("setScaleZ",&Transform3D::setScaleZ)
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
            luabind::value("KEY_RIGHT",SDLK_RIGHT)
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
    //bindAnimationInstance();
    //bindAssimpModelInstance();
    //bindLightInstance();
    //bindShaderInstance();
    //bindSpriteInstance();
    //bindPhysicsObjectInstane();
  }

  void LuaComponent::bindAudioInstance() {
    luabind::module(mState) [
        luabind::class_<AudioInstance>("AudioInstance")
        .def("play",&AudioInstance::play)
        .def("pause",&AudioInstance::pause)
        .def("stop",&AudioInstance::stop)
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
      luabind::object funq = table["update"];
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
      luabind::object funq = table["handleInput"];
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
  str << "lua> run-time error: " << msg;
  cout << str.str() << endl;
  // log the callstack
  std::string traceback = luabind::call_function<std::string>( luabind::globals(L)["debug"]["traceback"] );
  traceback = std::string( "lua> " ) + traceback;
  cout << traceback.c_str() << endl;
  // return unmodified error object
  return 1;
}
