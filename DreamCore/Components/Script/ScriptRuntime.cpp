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
#include "Project/ProjectRuntime.h"

#define SOL_CHECK_ARGUMENTS 1
#define SOL_ALL_SAFETIES_ON 1
#include <sol.h>



namespace octronic::dream
{


    ScriptRuntime::ScriptRuntime
    (ScriptDefinition* definition, ProjectRuntime* rt)
        : SharedAssetRuntime("ScriptRuntime",definition,rt),
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
        if(dreamTryLock()) {
            dreamLock();
            auto path = getAssetFilePath();
            LOG_DEBUG( "ScriptRuntime: Script at {}" , path);
            return true;
        } dreamElseLockFailed
    }

    bool
    ScriptRuntime::createEntityState
    (EntityRuntime* entity)
    {
        if(dreamTryLock()) {
            dreamLock();
            ScriptComponent* scriptComponent = mProjectRuntime->getScriptComponent();
            return scriptComponent->createEntityState(this, entity);
        } dreamElseLockFailed
    }

    bool
    ScriptRuntime::removeEntityState
    (UuidType uuid)
    {
        if(dreamTryLock()) {
            dreamLock();
            ScriptComponent* scriptComponent = mProjectRuntime->getScriptComponent();
            return scriptComponent->removeEntityState(uuid);
        } dreamElseLockFailed
    }

    string
    ScriptRuntime::getSource
    ()
    {
        if(dreamTryLock()) {
            dreamLock();
            return mSource;
        } dreamElseLockFailed
    }

    void
    ScriptRuntime::setSource
    (const string& source)
    {
        if(dreamTryLock()) {
            dreamLock();
            mSource = source;
        } dreamElseLockFailed
    }

    // Function Execution =======================================================

    bool
    ScriptRuntime::executeOnUpdate
    (EntityRuntime* entity)
    {
        if(dreamTryLock()) {
            dreamLock();
            ScriptComponent* scriptComponent = mProjectRuntime->getScriptComponent();
            return scriptComponent->executeScriptOnUpdate(this, entity);
        } dreamElseLockFailed
    }

    bool
    ScriptRuntime::executeOnInit
    (EntityRuntime* entity)
    {
        if(dreamTryLock()) {
            dreamLock();
            ScriptComponent* scriptComponent = mProjectRuntime->getScriptComponent();
            return scriptComponent->executeScriptOnInit(this, entity);
        } dreamElseLockFailed
    }

    bool
    ScriptRuntime::executeOnEvent
    (EntityRuntime* entity)
    {
        if(dreamTryLock()) {
            dreamLock();
            ScriptComponent* scriptComponent = mProjectRuntime->getScriptComponent();
            return scriptComponent->executeScriptOnEvent(this, entity);
        } dreamElseLockFailed
    }

    bool
    ScriptRuntime::executeOnInput
    (InputComponent* inputComp, SceneRuntime* sr)
    {
        if(dreamTryLock()) {
            dreamLock();
            ScriptComponent* scriptComponent = mProjectRuntime->getScriptComponent();
            return scriptComponent->executeScriptOnInput(this, inputComp, sr);
        } dreamElseLockFailed
    }


    bool ScriptRuntime::registerInputScript()
    {
        if(dreamTryLock()) {
            dreamLock();
            ScriptComponent* scriptComponent = mProjectRuntime->getScriptComponent();
            return scriptComponent->registerInputScript(this);
        } dreamElseLockFailed
    }

    bool ScriptRuntime::removeInputScript()
    {
        if(dreamTryLock()) {
            dreamLock();
            ScriptComponent* scriptComponent = mProjectRuntime->getScriptComponent();
            return scriptComponent->removeInputScript(this);
        } dreamElseLockFailed
    }
}
