/*
 * LuaEngine
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

#include "LuaEngine.h"

#include <sstream>
#include <luabind/adopt_policy.hpp>

#include "LuaScriptCache.h"
#include "../Math.h"

using namespace std;

int errorHandler(lua_State *L)
{
    // log the error message
    luabind::object msg(luabind::from_stack( L, -1 ));
    std::ostringstream str;
    str << "Lua - RuntimeError: " << msg;
    cerr << str.str() << endl;
    // log the callstack
    std::string traceback = luabind::call_function<std::string>( luabind::globals(L)["debug"]["traceback"] );
    traceback = std::string( "Lua - Traceback: " ) + traceback;
    cerr << traceback.c_str() << endl;
    // return unmodified error object
    return 1;
}

namespace Dream
{
    LuaEngine::LuaEngine()
    {
        mState = nullptr;
    }

    LuaEngine::~LuaEngine()
    {
        if (DEBUG)
        {
            cout << "LuaEngine: Destroying Object" << endl;
        }

        if (mState != nullptr)
        {
            lua_close(mState);
        }
    }

    bool LuaEngine::init()
    {
        if (DEBUG)
        {
            cout << "LuaEngine: Initialising LuaEngine" << endl;
        }
        mState = luaL_newstate();
        luabind::open(mState);
        luaopen_base(mState);
        luabind::set_pcall_callback(&errorHandler);
        luaL_dostring(mState, mScriptLoadFromFile.c_str());
        luaL_dostring(mState, mScriptLoadFromString.c_str());
        return mState != nullptr;
    }

    bool LuaEngine::createAllScripts()
    {
        if (VERBOSE)
        {
            cout << "LuaEngine: CreateAllScripts Called" << endl;
        }
        map<SceneObject*,LuaScriptInstance*>::iterator scriptIt;
        for (scriptIt=mScriptMap->begin(); scriptIt != mScriptMap->end(); scriptIt++)
        {

            SceneObject *sceneObject = scriptIt->first;
            LuaScriptInstance *luaScript = scriptIt->second;

            if (luaScript == nullptr)
            {
                cerr << "LuaEngine: Load Failed, LuaScriptInstance is NULL" << endl;
                continue;
            }

            if (sceneObject == nullptr)
            {
                cerr << "LuaEngine: Load Failed, SceneObject is NULL" << endl;
                continue;
            }

            if (luaScript->getLoadedFlag())
            {
                continue;
            }

            if (sceneObject->getDeleteFlag())
            {

                if (DEBUG)
                {
                    cout << "LuaEngine: SceneObject marked for deletion, not loading script." << endl;
                }
                continue;
            }

            if (DEBUG)
            {
                cout << "LuaEngine: loading script '" << luaScript->getName() << "' for '" << sceneObject->getName() << "'" << endl;
                cout << "LuaEngine: Loading Lua script from " << luaScript->getAbsolutePath()
                     << endl << flush;
            }

            if (!loadScript(sceneObject, luaScript))
            {
                return false;
            }

            if (DEBUG)
            {
                cout << "LuaEngine: Loaded " << sceneObject->getUuid() << " Successfully" << endl;
            }

            luaScript->setLoadedFlag(true);
            executeScriptInit(sceneObject);
        }

        return true;
    }

    bool LuaEngine::loadScript(SceneObject* sceneObject, LuaScriptInstance* scriptInstance)
    {
        string id = sceneObject->getUuid();
        if (DEBUG)
        {
            cout << "LuaEngine: loadScript called for " << id << endl;
        }
        luabind::object newtable = luabind::newtable(mState);
        try
        {
            string path = scriptInstance->getAbsolutePath();
            string script = LuaScriptCache::getScript(path);

            // Old way
            //luabind::call_function<void>(mState, "scriptLoadFromFile", newtable, path.c_str());

            // With Cache...
            luabind::call_function<void>(mState, "scriptLoadFromString", newtable, script.c_str());

            luabind::object reg = luabind::registry(mState);
            reg[id] = newtable;
        }
        catch (luabind::error e)
        {
            cerr << "LuaEngine: loadScript exception:" << endl
                 << "\t" << e.what() << endl;
            return false;
        }
        return true;
    }


    void LuaEngine::setLuaScriptMap(map<SceneObject*,LuaScriptInstance*>* scriptMap) {
        mScriptMap = scriptMap;
    }

    void LuaEngine::stackDump() {
        cerr << "LuaEngine: Stack Dump!" << endl;
        int i;
        int top = lua_gettop(mState);
        // repeat for each level
        for (i = 1; i <= top; i++) {
            int t = lua_type(mState, i);
            switch (t) {
                // strings
                case LUA_TSTRING:
                    cerr << lua_tostring(mState, i);
                    break;
                    // booleans
                case LUA_TBOOLEAN:
                    cerr << (lua_toboolean(mState, i) ? "true" : "false");
                    break;
                    // numbers
                case LUA_TNUMBER:
                    cerr << lua_tonumber(mState, i);
                    break;
                    // other values
                default:
                    cout << lua_typename(mState, t);
                    break;
            }
            // put a separator
            cerr << " ";
        }
        // end the listing
        cerr << endl;
    }

    bool LuaEngine::update() {
        if (VERBOSE) {
            cout << "LuaEngine: Update Called" << endl;
        }
        map<SceneObject*,LuaScriptInstance*>::iterator scriptIt;
        for (scriptIt=mScriptMap->begin(); scriptIt != mScriptMap->end(); scriptIt++) {
            SceneObject* key = scriptIt->first;
            if (key->getDeleteFlag()) {
                if (DEBUG) {
                    cout << "LuaEngine: Skipping update on " << key->getUuid() << endl;
                }
                continue;
            }
            LuaScriptInstance *value = scriptIt->second;
            if (!executeScriptUpdate(key)) {
                return false;
            }
            if (key->hasFocus()) {
                if (!executeScriptKeyHandler(key)) {
                    return false;
                }
            }
            if (key->hasEvents()) {
                if (!executeScriptEventHandler(key)) {
                    return false;
                }
            }
        }
        return true;
    }

    bool LuaEngine::executeScriptUpdate(SceneObject* sceneObject) {
        string id = sceneObject->getUuid();
        if (VERBOSE) {
            cout << "LuaEngine: Calling onUpdate for " << sceneObject->getNameUuidString() << endl;
        }
        try {
            luabind::object reg = luabind::registry(mState);
            luabind::object table = reg[id];
            luabind::object funq = table[LUA_SCRIPT_ON_UPDATE_FUNCTION];
            luabind::call_function<void>(funq,sceneObject);
        } catch (luabind::error &e) {
            string error = lua_tostring( e.state(), -1 );
            cerr << "LuaEngine: onUpdate exception:" << endl << e.what() << endl;
            cerr << error << endl;

            return false;
        }
        return true;
    }

    bool LuaEngine::executeScriptInit(SceneObject* sceneObject) {
        string id = sceneObject->getUuid();
        if (VERBOSE) {
            cout << "LuaEngine: Calling onInit for " << sceneObject->getNameUuidString() << endl << flush;
        }
        try {
            luabind::object reg = luabind::registry(mState);
            luabind::object table = reg[id];
            luabind::object funq = table[LUA_SCRIPT_ON_INIT_FUNCTION];
            luabind::call_function<void>(funq,sceneObject);
        } catch (luabind::error &e) {
            cerr << "LuaEngine: onInit exception:" << endl << e.what() << endl;
            return false;
        }
        return true;
    }

    bool LuaEngine::executeScriptKeyHandler(SceneObject* sceneObject) {
        string id = sceneObject->getUuid();
        if (VERBOSE) {
            cout << "LuaEngine: Calling onInput for " << sceneObject->getNameUuidString() << endl << flush;
        }
        try {
            luabind::object reg = luabind::registry(mState);
            luabind::object table = reg[id];
            luabind::object funq = table[LUA_SCRIPT_ON_INPUT_FUNCTION];
            vector<SDL_Event>::iterator it;
            for (it=mSDLEvents.begin();it!=mSDLEvents.end();it++) {
                SDL_Event next = (*it);
                luabind::call_function<void>(funq,sceneObject,next);
            }
        } catch (luabind::error &e) {
            cerr << "LuaEngine: onInput exception:" << endl << e.what() << endl;
            return false;
        }
        return true;
    }

    bool LuaEngine::executeScriptEventHandler(SceneObject* sceneObject) {
        string id = sceneObject->getUuid();
        if (VERBOSE) {
            cout << "LuaEngine: Calling onEvent for " << sceneObject->getNameUuidString() << endl << flush;
        }
        try {
            luabind::object reg = luabind::registry(mState);
            luabind::object table = reg[id];
            luabind::object funq = table[LUA_SCRIPT_ON_EVENT_FUNCTION];
            vector<Event*>* events = sceneObject->getEventQueue();
            vector<Event*>::iterator eventIt;
            for (eventIt=events->begin();eventIt!=events->end(); eventIt++) {
                luabind::call_function<void>(funq,sceneObject,(*eventIt));
            }
            sceneObject->cleanupEvents();
        } catch (luabind::error &e) {
            cerr << "LuaEngine: onEvent exception:" << endl << e.what() << endl;
            return false;
        }
        return true;
    }

    void LuaEngine::setSDL_Events(vector<SDL_Event> events)
    {
        mSDLEvents = events;
    }

} // End of Dream

