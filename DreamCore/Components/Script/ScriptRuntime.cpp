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
#include "Storage/StorageManager.h"
#include "Storage/File.h"
#include "Scene/SceneRuntime.h"
#include "Entity/EntityRuntime.h"
#include "Project/ProjectRuntime.h"

#define SOL_CHECK_ARGUMENTS 1
#define SOL_ALL_SAFETIES_ON 1
#include <sol.h>


namespace octronic::dream
{
    ScriptRuntime::ScriptRuntime
    (ProjectRuntime* rt, ScriptDefinition* definition)
        : SharedAssetRuntime(rt, definition),
          mSource("")
    {
        LOG_TRACE( "ScriptRuntime: {} {}",__FUNCTION__, getNameAndUuidString());
        return;
    }

    ScriptRuntime::~ScriptRuntime
    ()
    {
        LOG_TRACE("ScriptRuntime: {} {}",__FUNCTION__, mDefinitionHandle->getNameAndUuidString());
    }

    bool
    ScriptRuntime::loadFromDefinition
    ()
    {
        auto path = getAssetFilePath();
        LOG_DEBUG( "ScriptRuntime: Script at {}" , path);

        StorageManager* sm = mProjectRuntimeHandle->getStorageManager();
        File* scriptFile = sm->openFile(path);

        if (!scriptFile->exists())
        {
            LOG_ERROR("ScriptCache: Script file does not exist");
            setSource("");
            sm->closeFile(scriptFile);
            mLoadError = true;
            return false;
        }
        else
        {
            setSource(scriptFile->readString());
            mLoaded = true;
        }

        sm->closeFile(scriptFile);
        return true;
    }

    bool
    ScriptRuntime::createEntityState
    (EntityRuntime* entity)
    {
        ScriptComponent* scriptComponent = mProjectRuntimeHandle->getScriptComponent();
        return scriptComponent->createEntityState(this, entity);
    }

    bool
    ScriptRuntime::removeEntityState
    (UuidType uuid)
    {
        ScriptComponent* scriptComponent = mProjectRuntimeHandle->getScriptComponent();
        return scriptComponent->removeEntityState(uuid);
    }

    string
    ScriptRuntime::getSource
    ()
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
        ScriptComponent* scriptComponent = mProjectRuntimeHandle->getScriptComponent();
        return scriptComponent->executeScriptOnUpdate(this, entity);
    }

    bool
    ScriptRuntime::executeOnInit
    (EntityRuntime* entity)
    {
        ScriptComponent* scriptComponent = mProjectRuntimeHandle->getScriptComponent();
        return scriptComponent->executeScriptOnInit(this, entity);
    }

    bool
    ScriptRuntime::executeOnEvent
    (EntityRuntime* entity)
    {
        ScriptComponent* scriptComponent = mProjectRuntimeHandle->getScriptComponent();
        return scriptComponent->executeScriptOnEvent(this, entity);

    }

    bool
    ScriptRuntime::executeOnInput
    (InputComponent* inputComp, SceneRuntime* sr)
    {
        ScriptComponent* scriptComponent = mProjectRuntimeHandle->getScriptComponent();
        return scriptComponent->executeScriptOnInput(this, inputComp, sr);
    }


    bool ScriptRuntime::registerInputScript()
    {
        ScriptComponent* scriptComponent = mProjectRuntimeHandle->getScriptComponent();
        return scriptComponent->registerInputScript(this);
    }

    bool ScriptRuntime::removeInputScript()
    {
        ScriptComponent* scriptComponent = mProjectRuntimeHandle->getScriptComponent();
        return scriptComponent->removeInputScript(this);
    }


    void ScriptRuntime::pushNextTask()
    {
        auto taskQueue = mProjectRuntimeHandle->getTaskQueue();

        if (!mLoaded && !mLoadError)
        {
            if (mLoadFromDefinitionTask->hasState(TASK_STATE_QUEUED))
            {
                taskQueue->pushTask(mLoadFromDefinitionTask);
            }
        }
        else
        {
            if (mLoaded && !mLoadError && mLoadFromDefinitionTask->hasState(TASK_STATE_COMPLETED))
            {
                for(EntityRuntime* entity : mInstances)
                {
                    // Do entity specific tasks
                    if (entity->getSceneRuntime()->hasState(SCENE_STATE_ACTIVE))
                    {

                        // Not yet Initialised
                        if (!entity->getScriptInitialised())
                        {
                            taskQueue->pushTask(entity->getScriptCreateStateTask());
                        }
                        // Has been initialised
                        else if (entity->allRuntimesLoaded())
                        {
                            if (entity->getScriptOnInitTask()->hasState(TASK_STATE_QUEUED))
                            {
                                taskQueue->pushTask(entity->getScriptOnInitTask());
                            }
                            else
                            {
                                // Always push update task
                                taskQueue->pushTask(entity->getScriptOnUpdateTask());

                                // If there are events to process, push on event task
                                if (entity->hasEvents())
                                {
                                    taskQueue->pushTask(entity->getScriptOnEventTask());
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
