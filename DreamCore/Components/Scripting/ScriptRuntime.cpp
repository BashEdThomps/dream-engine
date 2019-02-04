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
#include "../../Project/ProjectRuntime.h"

namespace Dream
{
    ScriptRuntime::ScriptRuntime
    (ScriptDefinition* definition, ProjectRuntime* rt)
        : SharedAssetRuntime(definition,rt),
          mSource("")
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
        getLog()->trace("Destructing {}", mDefinition->getNameAndUuidString());
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

    ScriptRuntimeState*
    ScriptRuntime::createState
    (SceneObjectRuntime* runtime)
    {
        ScriptRuntimeState* s = new ScriptRuntimeState(this,runtime);
        s->error = false;
        s->runtime = runtime;
        #ifdef DREAM_LOG
        getLog()->debug("loadScript called for {}", runtime->getNameAndUuidString() );
        getLog()->debug("calling scriptLoadFromString in lua for {}" , runtime->getNameAndUuidString() );
        #endif

        mProjectRuntime->getScriptComponent()->lock();
        sol::state_view solStateView(ScriptComponent::State);
        sol::environment environment(ScriptComponent::State, sol::create, solStateView.globals());

        solStateView[s->runtime->getUuid()] = environment;

        auto exec_result = solStateView.safe_script
        (mSource, environment,
         [](lua_State*, sol::protected_function_result pfr){return pfr;});

        // it did not work
        if(!exec_result.valid())
        {
            // An error has occured
            sol::error err = exec_result;
            std::string what = err.what();
            #ifdef DREAM_LOG
            getLog()->critical("{}:\nCould not execute lua script:\n{}",
                s->runtime->getUuid(),what);
            #endif
            s->error = true;
        }
        mProjectRuntime->getScriptComponent()->unlock();

        if (!s->error)
        {
            #ifdef DREAM_LOG
            getLog()->debug("Loaded Script Successfully");
            #endif
        }
        s->initialised = false;
        return s;
    }

    void
    ScriptRuntime::removeState
    (ScriptRuntimeState* s)
    {
        mProjectRuntime->getScriptComponent()->lock();
        sol::state_view stateView(ScriptComponent::State);
        stateView[s->runtime->getUuid()] = sol::lua_nil;
        mProjectRuntime->getScriptComponent()->unlock();
        #ifdef DREAM_LOG
        getLog()->debug( "Removed script lua table for {}" , s->runtime->getNameAndUuidString());
        #endif
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
    (ScriptRuntimeState* state)
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        #endif
        mProjectRuntime->getScriptComponent()->lock();
        sol::state_view solStateView(ScriptComponent::State);
        if (state->error)
        {
            #ifdef DREAM_LOG
            getLog()->error("Cannot execute script {} in error state", getNameAndUuidString());
            #endif
            return false;
        }

        #ifdef DREAM_LOG
        getLog()->debug("Calling onUpdate for {}",state->runtime->getNameAndUuidString());
        #endif

        bool retval = false;
        sol::protected_function onUpdateFunction = solStateView[state->runtime->getUuid()][Constants::LUA_UPDATE_FUNCTION];
        auto result = onUpdateFunction(state->runtime);
        if (!result.valid())
        {
            // An error has occured
           sol::error err = result;
           std::string what = err.what();
           #ifdef DREAM_LOG
           getLog()->critical
            (
               "{}:\nCould not execute onUpdate in lua script:\n{}",
                state->runtime->getNameAndUuidString(),
                what
            );
            #endif
            state->error = true;
           retval =  false;
        }
        retval = true;
        mProjectRuntime->getScriptComponent()->unlock();
        return retval;
    }

    bool
    ScriptRuntime::executeOnInit
    (ScriptRuntimeState* state)
    {
        if (state->error)
        {
           #ifdef DREAM_LOG
            getLog()->error("Cannot init, script for {} in error state.",state->runtime->getNameAndUuidString());
            #endif
            return false;
        }
        if (state->initialised)
        {
           #ifdef DREAM_LOG
            getLog()->trace("Script has all ready been initialised for {}",state->runtime->getNameAndUuidString());
            #endif
            return false;
        }

        mProjectRuntime->getScriptComponent()->lock();
        sol::state_view solStateView(ScriptComponent::State);
        #ifdef DREAM_LOG
        getLog()->debug("Calling onInit in {} for {}",  getName(), state->runtime->getName());
        #endif
        sol::protected_function onInitFunction = solStateView[state->runtime->getUuid()][Constants::LUA_INIT_FUNCTION];
        auto initResult = onInitFunction(state->runtime);
        if (!initResult.valid())
        {
            // An error has occured
           sol::error err = initResult;
           std::string what = err.what();
           #ifdef DREAM_LOG
           getLog()->critical
           (
                "{}\nCould not execute onInit in lua script:\n{}",
                state->runtime->getNameAndUuidString(),
                what
            );
            #endif
           state->error = true;
        }
        else
        {
            state->initialised = true;
        }
        mProjectRuntime->getScriptComponent()->unlock();
        return true;
    }

    bool
    ScriptRuntime::executeOnEvent
    (ScriptRuntimeState* state)
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        #endif

        if (state->error)
        {
           #ifdef DREAM_LOG
            getLog()->error( "Cannot execute {} in error state ",  getNameAndUuidString());
            #endif
            state->runtime->clearEventQueue();
            return false;
        }


        if (!state->runtime->hasEvents())
        {
            return false;
        }

        #ifdef DREAM_LOG
        getLog()->debug( "Calling onEvent for {}", state->runtime->getNameAndUuidString());
        #endif

        mProjectRuntime->getScriptComponent()->lock();
        sol::state_view solStateView(ScriptComponent::State);
        sol::protected_function onEventFunction =
            solStateView[state->runtime->getUuid()][Constants::LUA_EVENT_FUNCTION];

        for (const Event& e : state->runtime->getEventQueue())
        {
            auto result = onEventFunction(state->runtime,e);
            if (!result.valid())
            {
                // An error has occured
               sol::error err = result;
               std::string what = err.what();
               #ifdef DREAM_LOG
               getLog()->error
                (
                   "{}:\nCould not execute onEvent in lua script:\n{}",
                    state->runtime->getNameAndUuidString(), what
                );
                #endif
               state->error = true;
               break;
            }
        }
        state->runtime->clearEventQueue();
        mProjectRuntime->getScriptComponent()->unlock();
        return true;
    }

    bool
    ScriptRuntime::executeOnInput
    (InputComponent* inputComp, SceneRuntime* sr)
    {
        #ifdef DREAM_LOG
        getLog()->trace("Executing onInput function");
        #endif

        bool retval;
        mProjectRuntime->getScriptComponent()->lock();
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
            retval = false;

        }
        mProjectRuntime->getScriptComponent()->unlock();
        retval = true;
        return retval;
    }


    bool
    ScriptRuntime::executeOnNanoVG
    (NanoVGComponent* nvg, SceneRuntime* sr)
    {
        bool retval = false;
        #ifdef DREAM_LOG
        auto log = getLog();
        getLog()->info( "Calling onNanoVG for {}" , getNameAndUuidString() );
        #endif
        mProjectRuntime->getScriptComponent()->lock();
        sol::state_view solStateView(ScriptComponent::State);
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
           retval = false;
        }

        mProjectRuntime->getScriptComponent()->unlock();
        retval = true;
        return retval;
    }

    void
    ScriptRuntime::registerInputScript
    ()
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        #endif
        mProjectRuntime->getScriptComponent()->lock();
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

        mProjectRuntime->getScriptComponent()->unlock();
    }

    void
    ScriptRuntime::registerNanoVGScript
    ()
    {
        #ifdef DREAM_LOG
        auto log = getLog();
        #endif

        mProjectRuntime->getScriptComponent()->lock();
        sol::state_view solStateView(ScriptComponent::State);
        sol::environment environment(ScriptComponent::State, sol::create, solStateView.globals());
        solStateView[getUuid()] = environment;
        auto exec_result = solStateView.safe_script
        (mSource, environment,[](lua_State*, sol::protected_function_result pfr)
        {
                return pfr;
        });
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
        mProjectRuntime->getScriptComponent()->unlock();
    }
}
