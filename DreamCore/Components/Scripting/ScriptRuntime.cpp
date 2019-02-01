/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptRuntime.h"
#include "ScriptDefinition.h"
#include "ScriptComponent.h"

#include "../Input/InputComponent.h"
#include "../Graphics/NanoVGComponent.h"
#include "../../Scene/SceneRuntime.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"
#include "../../deps/sol2/sol.hpp"

namespace Dream
{
    ScriptRuntime::ScriptRuntime
    (ScriptDefinition* definition, ProjectRuntime* rt)
        : SharedAssetRuntime(definition,rt),
          mSource(""),
          mError(false)
    {
        #ifdef DREAM_LOG
        setLogClassName("ScriptRuntime");
        getLog()->trace( "Constructing {}", getNameAndUuidString());
        #endif
        return;
    }

    ScriptRuntime::~ScriptRuntime
    ()
    {
        #ifdef DREAM_LOG
        getLog()->trace("Destructing {}", mDefinition->getNameAndUuidString() );
        #endif
    }

    bool
    ScriptRuntime::useDefinition
    ()
    {
        auto path = getAssetFilePath();
        #ifdef DREAM_LOG
        getLog()->debug( "Script at {}" , path);
        #endif
        return true;
    }

    void
    ScriptRuntime::addRuntime
    (SceneObjectRuntime* sor)
    {
        auto target = mRuntimes.end();
        for (auto itr = mRuntimes.begin(); itr != mRuntimes.end(); itr++)
        {
            if ((*itr).runtime == sor)
            {
                target = itr;
                break;
            }
        }

        bool error = false;
        if (target == mRuntimes.end())
        {
            uint32_t id = sor->getUuid();
            #ifdef DREAM_LOG
            getLog()->debug( "loadScript called for {}", id );
            getLog()->debug( "calling scriptLoadFromString in lua for {}" , id );
            #endif

            sol::state_view solStateView(ScriptComponent::State);
            sol::environment environment(ScriptComponent::State, sol::create, solStateView.globals());

            solStateView[sor->getUuid()] = environment;

            auto exec_result = solStateView.safe_script
            (   mSource, environment,
                [](lua_State*, sol::protected_function_result pfr)
                {
                    // pfr will contain things that went wrong, for either loading or executing the script
                    // Can throw your own custom error
                    // You can also just return it, and let the call-site handle the error if necessary.
                    return pfr;
                }
            );

            // it did not work
            if(!exec_result.valid())
            {
                // An error has occured
                sol::error err = exec_result;
                std::string what = err.what();
                #ifdef DREAM_LOG
                getLog()->critical(
                    "{}:\n"
                    "Could not execute lua script:\n"
                    "{}",
                    sor->getNameAndUuidString(),
                    what
                );
                #endif
                error = true;
            }

            #ifdef DREAM_LOG
            getLog()->debug("Loaded Script Successfully");
            #endif
            auto pair = ScriptRuntimeState();
            pair.runtime = sor;
            pair.initialised = false;
            pair.error = error;
            mRuntimes.push_back(pair);
        }
    }

    void
    ScriptRuntime::removeRuntime
    (SceneObjectRuntime* sor)
    {
        auto target = mRuntimes.end();

        for (auto itr = mRuntimes.begin(); itr != mRuntimes.end(); itr++)
        {
            if ((*itr).runtime == sor)
            {
                target = itr;
                break;
            }
        }

        if (target != mRuntimes.end())
        {
            sol::state_view stateView(ScriptComponent::State);
            stateView[sor->getUuid()] = sol::lua_nil;
            #ifdef DREAM_LOG
            getLog()->debug( "Removed script lua table for {}" , sor->getNameAndUuidString());
            #endif
            mRuntimes.erase(target);
        }
    }

    string
    ScriptRuntime::getSource
    ()
    const
    {
        return mSource;
    }

    void
    ScriptRuntime::setSource
    (const string& source)
    {
        mSource = source;
    }

    // Function Execution =======================================================

    bool
    ScriptRuntime::executeOnUpdate
    ()
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        #endif
        sol::state_view solStateView(ScriptComponent::State);
        for (auto& sceneObject : mRuntimes)
        {
            if (sceneObject.error)
            {
                #ifdef DREAM_LOG
                getLog()->error("Cannot execute {} in error state", getNameAndUuidString());
                #endif
                return false;
            }

            #ifdef DREAM_LOG
            getLog()->debug("Calling onUpdate for {}",sceneObject.runtime->getNameAndUuidString() );
            #endif

            sol::protected_function onUpdateFunction = solStateView[sceneObject.runtime->getUuid()][Constants::LUA_UPDATE_FUNCTION];
            auto result = onUpdateFunction(sceneObject.runtime);
            if (!result.valid())
            {
                // An error has occured
               sol::error err = result;
               std::string what = err.what();
               #ifdef DREAM_LOG
               getLog()->critical
                (
                   "{}:\nCould not execute onUpdate in lua script:\n{}",
                    sceneObject.runtime->getNameAndUuidString(),
                    what
                );
                #endif
               sceneObject.error = true;
               return false;
            }
        }
        return true;
    }

    bool
    ScriptRuntime::executeOnInit
    ()
    {

        sol::state_view solStateView(ScriptComponent::State);
        for (auto& sceneObject : mRuntimes)
        {
            if (sceneObject.error)
            {
               #ifdef DREAM_LOG
                getLog()->error("Cannot init, script for {} in error state.",sceneObject.runtime->getNameAndUuidString());
                #endif
                continue;
            }
            if (sceneObject.initialised)
            {
               #ifdef DREAM_LOG
                getLog()->trace("Script has all ready been initialised for {}",sceneObject.runtime->getNameAndUuidString());
                #endif
                continue;
            }
               #ifdef DREAM_LOG
               getLog()->debug("Calling onInit in {} for {}",  getName(),  sceneObject.runtime->getName());
               #endif
            sol::protected_function onInitFunction = solStateView[sceneObject.runtime->getUuid()][Constants::LUA_INIT_FUNCTION];
            auto initResult = onInitFunction(sceneObject.runtime);
            if (!initResult.valid())
            {
                // An error has occured
               sol::error err = initResult;
               std::string what = err.what();
               #ifdef DREAM_LOG
               getLog()->critical
               (
                    "{}\nCould not execute onInit in lua script:\n{}",
                    sceneObject.runtime->getNameAndUuidString(),
                    what
                );
                #endif
               sceneObject.error = true;
               continue;
            }
            sceneObject.initialised = true;
        }
        return true;
    }

    bool
    ScriptRuntime::executeOnInput
    (InputComponent* inputComp, SceneRuntime* sr)
    {
        #ifdef DREAM_LOG
        getLog()->trace("Executing onInput function");
        #endif
        sol::state_view solStateView(ScriptComponent::State);
        sol::protected_function onInputFunction = solStateView[getUuid()][Constants::LUA_INPUT_FUNCTION];
        auto result = onInputFunction(inputComp, sr);
        if (!result.valid())
        {
            // An error has occured
           sol::error err = result;
           std::string what = err.what();

            #ifdef DREAM_LOG
            getLog()->critical("Could not execute onInput in lua script:\n{}",what);
            #endif
           return false;
        }
       return true;
    }

    bool
    ScriptRuntime::executeOnEvent
    ()
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        #endif
        sol::state_view solStateView(ScriptComponent::State);
        for (auto& sceneObject : mRuntimes)
        {
            if (sceneObject.error)
            {

               #ifdef DREAM_LOG
                getLog()->error( "Cannot execute {} in error state ",  getNameAndUuidString());
                #endif
                sceneObject.runtime->clearEventQueue();
                return false;
            }

            if (!sceneObject.runtime->hasEvents())
            {
                continue;
            }

            #ifdef DREAM_LOG
            getLog()->debug( "Calling onEvent for {}", sceneObject.runtime->getNameAndUuidString());
            #endif
            sol::table objTable = solStateView[sceneObject.runtime->getUuid()];
            if (objTable == sol::lua_nil)
            {
                #ifdef DREAM_LOG
                getLog()->error("Object table is nil?");
                #endif
                return false;
            }
            sol::protected_function onEventFunction = objTable[Constants::LUA_EVENT_FUNCTION];
            for (const Event& e : sceneObject.runtime->getEventQueue())
            {
                auto result = onEventFunction(sceneObject.runtime,e);
                if (!result.valid())
                {
                    // An error has occured
                   sol::error err = result;
                   std::string what = err.what();
                   #ifdef DREAM_LOG
                   getLog()->error
                    (
                       "{}:\n"
                       "Could not execute onEvent in lua script:\n"
                       "{}",
                        sceneObject.runtime->getNameAndUuidString(),
                        what
                    );
                    #endif
                   sceneObject.error = true;
                   continue;
                }
            }
            sceneObject.runtime->clearEventQueue();
        }
        return true;
    }

    bool
    ScriptRuntime::executeOnNanoVG
    (NanoVGComponent* nvg, SceneRuntime* sr)
    {
        sol::state_view solStateView(ScriptComponent::State);
        #ifdef DREAM_LOG
        auto log = getLog();
        getLog()->info( "Calling onNanoVG for {}" , getNameAndUuidString() );
        #endif
        sol::protected_function onNanoVGFunction = solStateView[getUuid()][Constants::LUA_NANOVG_FUNCTION];
        auto initResult = onNanoVGFunction(nvg,sr);
        if (!initResult.valid())
        {
            // An error has occured
           sol::error err = initResult;
           std::string what = err.what();
           #ifdef DREAM_LOG
           getLog()->critical("Could not execute onNanoVG in lua script:\n{}",what);
           #endif
           return false;
        }
        return true;
    }

    void
    ScriptRuntime::registerInputScript
    ()
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        #endif
        sol::state_view solStateView(ScriptComponent::State);
        sol::environment environment(ScriptComponent::State, sol::create, solStateView.globals());
        solStateView[getUuid()] = environment;
        auto exec_result = solStateView.safe_script
        (   mSource, environment,
            [](lua_State*, sol::protected_function_result pfr)
            {
                return pfr;
            }
        );
        // it did not work
        if(!exec_result.valid())
        {
            // An error has occured
            sol::error err = exec_result;
            std::string what = err.what();
            #ifdef DREAM_LOG
            getLog()->critical("Could not execute lua script:\n{}",what);
            #endif
        }
        #ifdef DREAM_LOG
        getLog()->debug("Loaded Script Successfully");
        #endif
    }

    void
    ScriptRuntime::registerNanoVGScript
    ()
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        #endif
        sol::state_view solStateView(ScriptComponent::State);
        sol::environment environment(ScriptComponent::State, sol::create, solStateView.globals());
        solStateView[getUuid()] = environment;
        auto exec_result = solStateView.safe_script
        (   mSource, environment,
            [](lua_State*, sol::protected_function_result pfr)
            {
                return pfr;
            }
        );
        // it did not work
        if(!exec_result.valid())
        {
            // An error has occured
            sol::error err = exec_result;
            std::string what = err.what();

            #ifdef DREAM_LOG
            getLog()->critical("Could not execute lua script:\n{}",what);
            #endif
        }
        #ifdef DREAM_LOG
        getLog()->debug("Loaded Script Successfully");
        #endif
    }

}
