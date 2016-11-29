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
        luaopen_base(mState);
        bindAssetManager();
        bindComponents();
        bindProject();
        bindSceneObject();
        bindScene();
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
            .def("get_uuid",&SceneObject::getUUID)
        ];
    }

    void LuaComponent::bindTransform3D() {
      luabind::module(mState) [
          luabind::class_<Transform3D>("Transform3D")
          .def(luabind::constructor<>())
      ];
    }

    void LuaComponent::bindSDL() {
      bindSDL_Event();
    }

    void LuaComponent::bindSDL_Event() {
      luabind::module(mState) [
          luabind::class_<SDL_Event>("SDL_Event")
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
      } catch (exception e) {
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
        luabind::call_function<void>(funq,sceneObject);
      } catch (exception e) {
        cerr << "LuaComponent: handleInput exception:" << endl
             << "\t" << e.what() << endl;
        return false;
      }
      return true;
    }
} // End of Dream
