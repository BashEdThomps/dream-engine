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

#include "Components/Input/InputComponent.h"
#include "Scene/SceneRuntime.h"
#include "Scene/Entity/EntityRuntime.h"
#include <sol.h>
#include "Project/ProjectRuntime.h"

namespace octronic::dream
{
    const string ScriptRuntime::LUA_ON_INIT_FUNCTION = "onInit";
    const string ScriptRuntime::LUA_ON_UPDATE_FUNCTION = "onUpdate";
    const string ScriptRuntime::LUA_ON_INPUT_FUNCTION = "onInput";
    const string ScriptRuntime::LUA_ON_EVENT_FUNCTION = "onEvent";

    ScriptRuntime::ScriptRuntime
    (ScriptDefinition* definition, ProjectRuntime* rt)
        : SharedAssetRuntime(definition,rt),
          mSource("")
    {
        LOG_TRACE( "ScriptRuntime: {} {}",__FUNCTION__, getNameAndUuidString());
        return;
    }

    ScriptRuntime::~ScriptRuntime
    ()
    {
        LOG_TRACE("ScriptRuntime: {} {}",__FUNCTION__, mDefinition->getNameAndUuidString());
    }

    bool
    ScriptRuntime::useDefinition
    ()
    {
        auto path = getAssetFilePath();
        LOG_DEBUG( "ScriptRuntime: Script at {}" , path);
        return true;
    }

    bool
    ScriptRuntime::createEntityState
    (EntityRuntime* entity)
    {
        if (mProjectRuntime->getScriptComponent()->tryLock())
        {
            if (entity)
                LOG_DEBUG("ScriptRuntime: loadScript called for {}", entity->getNameAndUuidString() );
            LOG_DEBUG("ScriptRuntime: calling scriptLoadFromString in lua for {}" , entity->getNameAndUuidString() );
            {
                sol::state_view solStateView(ScriptComponent::State);

                // Create an environment for this entity runtime
                sol::environment environment(ScriptComponent::State, sol::create, solStateView.globals());
                solStateView[entity->getUuid()] = environment;

                auto exec_result = solStateView.safe_script(mSource, environment,
                                                            [](lua_State*, sol::protected_function_result pfr) {
                        return pfr;
            });

                // it did not work
                if(!exec_result.valid())
                {
                    // An error has occured
                    sol::error err = exec_result;
                    std::string what = err.what();
                    LOG_ERROR("ScriptRuntime: {} Could not execute lua script:\n{}",
                                 entity->getUuid(),what);
                    entity->setScriptError(true);
                    mProjectRuntime->getScriptComponent()->unlock();
                    assert(false);
                    return false;
                }
            }

            mProjectRuntime->getScriptComponent()->unlock();
            return true;
        }
        return false;
    }

    bool
    ScriptRuntime::removeEntityState
    (UuidType uuid)
    {
        if(mProjectRuntime->getScriptComponent()->tryLock())
        {
            sol::state_view stateView(ScriptComponent::State);
            stateView[uuid] = sol::lua_nil;
            mProjectRuntime->getScriptComponent()->unlock();
            LOG_DEBUG( "ScriptRuntime: Removed script lua table for {}" , uuid);
            return true;
        }
        return false;
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
    (EntityRuntime* entity)
    {
        if (mProjectRuntime->getScriptComponent()->tryLock())
        {
            sol::state_view solStateView(ScriptComponent::State);

            LOG_DEBUG("ScriptRuntime: Calling onUpdate for {}",entity->getNameAndUuidString());

            sol::protected_function onUpdateFunction = solStateView[entity->getUuid()][LUA_ON_UPDATE_FUNCTION];
            auto result = onUpdateFunction(entity);
            if (!result.valid())
            {
                // An error has occured
                sol::error err = result;
                string what = err.what();
                LOG_ERROR("ScriptRuntime: {} Could not execute onUpdate in lua script:\n{}",
                             entity->getNameAndUuidString(),
                             what);
                entity->setScriptError(true);
                mProjectRuntime->getScriptComponent()->unlock();
                assert(false);
                return false;
            }
            mProjectRuntime->getScriptComponent()->unlock();
            return true;
        }
        return false;
    }

    bool
    ScriptRuntime::executeOnInit
    (EntityRuntime* entity)
    {
        if (entity->getScriptError())
        {
            LOG_ERROR("ScriptRuntime: Cannot init, script for {} in error state.", entity->getNameAndUuidString());
            return true;
        }

        if (entity->getScriptInitialised())
        {
            LOG_TRACE("ScriptRuntime: Script has all ready been initialised for {}", entity->getNameAndUuidString());
            return true;
        }

        if(mProjectRuntime->getScriptComponent()->tryLock())
        {
            sol::state_view solStateView(ScriptComponent::State);
            LOG_DEBUG("ScriptRuntime: Calling onInit in {} for {}",  getName(), entity->getName());
            sol::protected_function onInitFunction = solStateView[entity->getUuid()][LUA_ON_INIT_FUNCTION];
            auto initResult = onInitFunction(entity);
            if (!initResult.valid())
            {
                // An error has occured
                sol::error err = initResult;
                string what = err.what();
                LOG_ERROR(
                            "ScriptRuntime: {}\nCould not execute onInit in lua script:\n{}",
                            entity->getNameAndUuidString(),
                            what
                            );
                entity->setScriptError(true);
                mProjectRuntime->getScriptComponent()->unlock();
                assert(false);
                return false;
            }
            else
            {
                entity->setScriptInitialised(true);
            }
            mProjectRuntime->getScriptComponent()->unlock();
            return true;
        }
        return false;
    }

    bool
    ScriptRuntime::executeOnEvent
    (EntityRuntime* entity)
    {
        if (entity->getScriptError())
        {
            LOG_ERROR( "ScriptRuntime: Cannot execute {} in error state ",  getNameAndUuidString());
            entity->clearEventQueue();
            return true;
        }

        if (!entity->hasEvents())
        {
            return true;
        }

        LOG_DEBUG( "ScriptRuntime: Calling onEvent for {}", entity->getNameAndUuidString());

        if(mProjectRuntime->getScriptComponent()->tryLock())
        {
            sol::state_view solStateView(ScriptComponent::State);
            sol::protected_function onEventFunction =
                    solStateView[entity->getUuid()][LUA_ON_EVENT_FUNCTION];

            for (const Event& e : *entity->getEventQueue())
            {
                auto result = onEventFunction(entity,e);
                if (!result.valid())
                {
                    // An error has occured
                    sol::error err = result;
                    string what = err.what();
                    LOG_ERROR("ScriptRuntime: {}:\nCould not execute onEvent in lua script:\n{}",
                              entity->getNameAndUuidString(), what);
                    entity->setScriptError(true);
                    mProjectRuntime->getScriptComponent()->unlock();
                    assert(false);
                    return false;
                }
            }
            entity->clearEventQueue();
            mProjectRuntime->getScriptComponent()->unlock();
            return true;
        }
        return false;
    }

    bool
    ScriptRuntime::executeOnInput
    (InputComponent* inputComp, SceneRuntime* sr)
    {
        LOG_TRACE("ScriptRuntime: Executing onInput function with {}",getUuid());

        if (mProjectRuntime->getScriptComponent()->tryLock())
        {
            sol::state_view solStateView(ScriptComponent::State);
            sol::protected_function onInputFunction = solStateView[getUuid()][LUA_ON_INPUT_FUNCTION];
            auto result = onInputFunction(inputComp, sr);
            if (!result.valid())
            {
                // An error has occured
                sol::error err = result;
                string what = err.what();
                LOG_ERROR("ScriptRuntime: Could not execute onInput in lua script:\n{}",what);
               	assert(false);
            	mProjectRuntime->getScriptComponent()->unlock();
                return false;
            }
            mProjectRuntime->getScriptComponent()->unlock();
            return true;
        }
        return false;
    }


    bool ScriptRuntime::registerInputScript()
    {
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
            string what = err.what();
            LOG_ERROR("ScriptRuntime: Could not execute lua script:\n{}",what);
            mProjectRuntime->getScriptComponent()->unlock();
            assert(false);
            return false;
        }
        LOG_DEBUG("ScriptRuntime: Loaded Input Script Successfully");
        mProjectRuntime->getScriptComponent()->unlock();
        return true;
    }

    bool ScriptRuntime::removeInputScript()
    {
        mProjectRuntime->getScriptComponent()->lock();
        sol::state_view solStateView(ScriptComponent::State);
        solStateView[getUuid()] = nullptr;
        LOG_DEBUG("ScriptRuntime: Removed input script Successfully");
        mProjectRuntime->getScriptComponent()->unlock();
        return true;
    }
}
