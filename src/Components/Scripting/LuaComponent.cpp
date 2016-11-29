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
        bindAssetManagerClass();
        bindComponentsClass();
        bindProjectClass();
        bindSceneClass();
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
        // Reset the stack
        lua_settop(mState,0);
        // File and Environment names
        string filename = scriptInstance->getAbsolutePath();
        string envName = scriptInstance->getUUID();
        // Load Script
        if (luaL_loadfile(mState, filename.c_str())) {
            cerr << "LuaComponent: lua_loadfile failed for " << filename << endl
                 << "LuaComponent: Lua says: " << lua_tostring(mState, -1) << endl;
            lua_pop(mState,1);
            return false;
        }
        // Create new ENV table
        lua_createtable(mState,0,1);
        // Push the print function to the env
        lua_getglobal(mState,"print");
        lua_setfield(mState,-2,"print");
        lua_pushvalue(mState,-1);
        // Add to registry
        lua_setfield(mState, LUA_REGISTRYINDEX, envName.c_str());
        // Set the function environment
        lua_setfenv(mState, -2);
        // Execute Script
        if  (lua_pcall(mState, 0, 0, 0)) {
            cerr << "LuaComponent: lua_pcall failed for " << filename << endl
                 << "LuaComponent: Lua says: " << lua_tostring(mState, -1) << endl;
            lua_pop(mState,1);
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
        // Reset the stack
        lua_settop(mState,0);
        // retrieve the environment from the resgistry
        lua_getfield(mState, LUA_REGISTRYINDEX, id.c_str());
        // get the desired function from the environment
        lua_getfield(mState, -1, "update");
        // Call the function
        lua_call(mState, 0, 0);
        return true;
    }

    bool LuaComponent::executeScriptKeyHandler(SceneObject* sceneObject, LuaScriptInstance* script) {
        // Reset the stack
        string id = script->getUUID();
        // retrieve the environment from the resgistry
        lua_settop(mState,0);
        lua_getfield(mState, LUA_REGISTRYINDEX, id.c_str());
        // get the desired function from the environment
        lua_getfield(mState, -1, "handleInput");
        // Call the function
        lua_call(mState, 0, 0);
        return true;
    }
} // End of Dream
