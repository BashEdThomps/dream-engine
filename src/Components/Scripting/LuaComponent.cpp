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

#include "LuaComponent.h"

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
    bindAssetManagerClass();
    bindComponentsClass();
    bindProjectClass();
    bindSceneClass();
    return loadScriptsFromMap();
  }

  bool LuaComponent::loadScriptsFromMap() {
    map<SceneObject*,LuaScriptInstance*>::iterator scriptIt;
    for (scriptIt=mScriptMap->begin(); scriptIt != mScriptMap->end(); scriptIt++) {
      SceneObject *key = scriptIt->first;
      LuaScriptInstance *value = scriptIt->second;
      if (!loadScript(value->getAbsolutePath())) {
        return false;
      }
      cout << "LuaComponent: Read Lua script from "
           << value->getAbsolutePath()
           << endl;
      stackDump(mState);
    }
    return true;
  }

  bool LuaComponent::loadScript(string filename) {
    if (luaL_loadfile(mState, filename.c_str()) || lua_pcall(mState, 0, 0, 0)) {
      cerr << "LuaComponent: Error script not loaded (" << filename << ")" << endl;
      return false;
    }
    return true;
  }

  void LuaComponent::bindAssetManagerClass() {
    luabind::module(mState) [
        luabind::class_<AssetManager>("AssetManager")
        .def(luabind::constructor<>())
    ];
  }

  void LuaComponent::bindComponentsClass() {

  }

  void LuaComponent::bindProjectClass() {
    luabind::module(mState) [
        luabind::class_<Project>("Project")
        .def(luabind::constructor<AssetManager*>())
    ];
  }

  void LuaComponent::bindSceneClass() {

  }

  void LuaComponent::setLuaScriptMap(map<SceneObject*,LuaScriptInstance*>* scriptMap) {
    mScriptMap = scriptMap;
  }

  void LuaComponent::stackDump (lua_State *L) {
    cout << "LuaComponent: Stack Dump!" << endl;
    int i;
    int top = lua_gettop(L);
    for (i = 1; i <= top; i++) {  /* repeat for each level */
      int t = lua_type(L, i);
      switch (t) {
        case LUA_TSTRING:  /* strings */
          cout << lua_tostring(L, i);
          break;
        case LUA_TBOOLEAN:  /* booleans */
          cout << (lua_toboolean(L, i) ? "true" : "false");
          break;
        case LUA_TNUMBER:  /* numbers */
          cout << lua_tonumber(L, i);
          break;
        default:  /* other values */
          cout << lua_typename(L, t);
          break;
      }
      cout << "  ";  /* put a separator */
    }
    cout << endl;  /* end the listing */
  }

} // End of Dream
