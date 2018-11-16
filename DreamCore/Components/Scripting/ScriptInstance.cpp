/*
 * LuaScriptInstance
 *
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

#include "ScriptInstance.h"
#include "ScriptDefinition.h"
#include "ScriptComponent.h"

#include "../../Scene/SceneObject/SceneObjectRuntime.h"
#include "../../deps/sol2/sol.hpp"

namespace Dream
{
    ScriptInstance::ScriptInstance
    (ScriptDefinition* definition, SceneObjectRuntime* transform)
        : IAssetInstance(definition,transform),
          mSource(""),
          mError(false)
    {
        setLogClassName("ScriptInstance");
        auto log = getLog();
        log->trace( "Constructing {}", getNameAndUuidString());
        return;
    }

    ScriptInstance::~ScriptInstance
    ()
    {
        auto log = getLog();
        log->trace("Destructing {}", mDefinition->getNameAndUuidString() );
    }

    bool
    ScriptInstance::load
    (string projectPath)
    {
        auto log = getLog();
        mAbsolutePath = projectPath + mDefinition->getAssetPath();
        log->debug( "Script at {}" , mAbsolutePath );
        return mAbsolutePath.size() != 0;
    }

    void
    ScriptInstance::update
    ()
    {

    }

    void
    ScriptInstance::addInstance
    (SceneObjectRuntime* sor)
    {
        auto log = getLog();
        auto target = mInstances.end();
        for (auto itr = mInstances.begin(); itr != mInstances.end(); itr++)
        {
            if ((*itr).runtime == sor)
            {
                target = itr;
                break;
            }
        }

        bool error = false;
        if (target == mInstances.end())
        {
            string id = sor->getUuid();
            log->debug( "loadScript called for {}", id );
            log->debug( "calling scriptLoadFromString in lua for {}" , id );

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
                log->critical(
                    "{}:\n"
                    "Could not execute lua script:\n"
                    "{}",
                    sor->getNameAndUuidString(),
                    what
                );
                error = true;
            }
            log->debug("Loaded Script Successfully");
            auto pair = ScriptInstanceState();
            pair.runtime = sor;
            pair.initialised = false;
            pair.error = error;
            mInstances.push_back(pair);
        }
    }

    void
    ScriptInstance::removeInstance
    (SceneObjectRuntime* sor)
    {
        auto log = getLog();
        auto target = mInstances.end();

        for (auto itr = mInstances.begin(); itr != mInstances.end(); itr++)
        {
            if ((*itr).runtime == sor)
            {
                target = itr;
                break;
            }
        }

        if (target != mInstances.end())
        {
            sol::state_view stateView(ScriptComponent::State);
            stateView[sor->getUuid()] = sol::lua_nil;
            log->debug( "Removed script lua table for {}" , sor->getNameAndUuidString());
            mInstances.erase(target);
        }
    }

    string&
    ScriptInstance::getSource
    ()
    {
        return mSource;
    }

    void
    ScriptInstance::setSource
    (string source)
    {
        mSource = source;
    }

    // Function Execution =======================================================

    bool
    ScriptInstance::executeOnUpdate
    ()
    {
        auto log = getLog();
        sol::state_view solStateView(ScriptComponent::State);
        for (auto& sceneObject : mInstances)
        {
            if (sceneObject.error)
            {
                log->error("Cannot execute {} in error state", getNameAndUuidString());
                return false;
            }

            log->debug("Calling onUpdate for {}",sceneObject.runtime->getNameAndUuidString() );
            sol::protected_function onUpdateFunction = solStateView[sceneObject.runtime->getUuid()][Constants::LUA_UPDATE_FUNCTION];
            auto result = onUpdateFunction(sceneObject.runtime);
            if (!result.valid())
            {
                // An error has occured
               sol::error err = result;
               std::string what = err.what();
               log->critical
                (
                   "{}:\nCould not execute onUpdate in lua script:\n{}",
                    sceneObject.runtime->getNameAndUuidString(),
                    what
                );
               sceneObject.error = true;
               return false;
            }
        }
        return true;
    }

    bool
    ScriptInstance::executeOnInit
    ()
    {
        auto log = getLog();
        sol::state_view solStateView(ScriptComponent::State);
        for (auto& sceneObject : mInstances)
        {
            if (sceneObject.error)
            {
                log->error("Cannot init, script for {} in error state.",sceneObject.runtime->getNameAndUuidString());
                continue;
            }
            if (sceneObject.initialised)
            {
                log->trace("Script has all ready been initialised for {}",sceneObject.runtime->getNameAndUuidString());
                continue;
            }
            log->debug("Calling onInit in {} for {}",  getName(),  sceneObject.runtime->getName());
            sol::protected_function onInitFunction = solStateView[sceneObject.runtime->getUuid()][Constants::LUA_INIT_FUNCTION];
            auto initResult = onInitFunction(sceneObject.runtime);
            if (!initResult.valid())
            {
                // An error has occured
               sol::error err = initResult;
               std::string what = err.what();
               log->critical
               (
                    "{}\nCould not execute onInit in lua script:\n{}",
                    sceneObject.runtime->getNameAndUuidString(),
                    what
                );
               sceneObject.error = true;
               continue;
            }
            sceneObject.initialised = true;
        }
        return true;
    }

    bool
    ScriptInstance::executeOnInput
    ()
    {
        auto log = getLog();
        sol::state_view solStateView(ScriptComponent::State);
        for (auto& sceneObject : mInstances)
        {
            if (sceneObject.error)
            {
                log->error( "Cannot execute {} in error state ",  sceneObject.runtime->getNameAndUuidString());
                continue;
            }

            if (!sceneObject.runtime->hasFocus())
            {
                continue;
            }

            log->debug("Calling onInput for {} (Has Focus) {}", sceneObject.runtime->getNameAndUuidString());
            sol::protected_function onInputFunction = solStateView[sceneObject.runtime->getUuid()][Constants::LUA_INPUT_FUNCTION];
            auto result = onInputFunction(sceneObject.runtime);
            if (!result.valid())
            {
                // An error has occured
               sol::error err = result;
               std::string what = err.what();
               log->critical
                (
                    "{}:\n"
                    "Could not execute onInput in lua script:\n"
                    "{}",
                    sceneObject.runtime->getNameAndUuidString(),
                    what
                );
               sceneObject.error = true;
            }
        }
        return true;
    }

    bool
    ScriptInstance::executeOnEvent
    ()
    {
        auto log = getLog();
        sol::state_view solStateView(ScriptComponent::State);
        for (auto& sceneObject : mInstances)
        {
            if (sceneObject.error)
            {
                log->error( "Cannot execute {} in error state ",  getNameAndUuidString());
                sceneObject.runtime->clearEventQueue();
                return false;
            }

            if (!sceneObject.runtime->hasEvents())
            {
                continue;
            }

            log->debug( "Calling onEvent for {}", sceneObject.runtime->getNameAndUuidString());
            sol::protected_function onEventFunction = solStateView[sceneObject.runtime->getUuid()][Constants::LUA_EVENT_FUNCTION];
            for (const Event& e : sceneObject.runtime->getEventQueue())
            {
                auto result = onEventFunction(sceneObject.runtime,e);
                if (!result.valid())
                {
                    // An error has occured
                   sol::error err = result;
                   std::string what = err.what();
                   log->error
                    (
                       "{}:\n"
                       "Could not execute onEvent in lua script:\n"
                       "{}",
                        sceneObject.runtime->getNameAndUuidString(),
                        what
                    );
                   sceneObject.error = true;
                   continue;
                }
            }
            sceneObject.runtime->clearEventQueue();
        }
        return true;
    }

    bool
    ScriptInstance::executeOnNanoVG
    ()
    {
        auto log = getLog();
        sol::state_view solStateView(ScriptComponent::State);
        for (auto& sceneObject : mInstances)
        {
            if (sceneObject.error)
            {
                log->error( "Cannot execute NanoVG {} in error state", sceneObject.runtime->getNameAndUuidString());
                return false;
            }
            log->info( "Calling onNanoVG for {}" , sceneObject.runtime->getNameAndUuidString() );
            sol::protected_function onNanoVGFunction = solStateView[sceneObject.runtime->getUuid()][Constants::LUA_NANOVG_FUNCTION];
            auto initResult = onNanoVGFunction(sceneObject.runtime);

            if (!initResult.valid())
            {
                // An error has occured
               sol::error err = initResult;
               std::string what = err.what();
               log->critical
               (
                    "{}\nCould not execute onNanoVG in lua script:\n{}",
                    sceneObject.runtime->getNameAndUuidString(),
                    what
                );
               sceneObject.error = true;
            }
        }
        return true;
    }

} // End of Dream
