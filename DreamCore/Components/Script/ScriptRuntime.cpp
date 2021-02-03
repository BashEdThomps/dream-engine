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

using std::unique_lock;

namespace octronic::dream
{


    ScriptRuntime::ScriptRuntime
    (ScriptDefinition* definition, ProjectRuntime* rt)
        : SharedAssetRuntime("ScriptRuntime",definition,rt),
          mSource("")
    {
        const unique_lock<mutex>lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();

        LOG_TRACE( "ScriptRuntime: {} {}",__FUNCTION__, getNameAndUuidString());
        return;
    }

    ScriptRuntime::~ScriptRuntime
    ()
    {
        const unique_lock<mutex>lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        LOG_TRACE("ScriptRuntime: {} {}",__FUNCTION__, mDefinition->getNameAndUuidString());
    }

    bool
    ScriptRuntime::useDefinition
    ()
    {
        const unique_lock<mutex>lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        auto path = getAssetFilePath();
        LOG_DEBUG( "ScriptRuntime: Script at {}" , path);
        return true;
    }

    bool
    ScriptRuntime::createEntityState
    (EntityRuntime* entity)
    {
        const unique_lock<mutex>lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        ScriptComponent* scriptComponent = mProjectRuntime->getScriptComponent();
		return scriptComponent->createEntityState(this, entity);
    }

    bool
    ScriptRuntime::removeEntityState
    (UuidType uuid)
    {
        const unique_lock<mutex>lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        ScriptComponent* scriptComponent = mProjectRuntime->getScriptComponent();
		return scriptComponent->removeEntityState(uuid);
    }

    string
    ScriptRuntime::getSource
    ()
    {
        const unique_lock<mutex>lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        return mSource;
    }

    void
    ScriptRuntime::setSource
    (const string& source)
    {
        const unique_lock<mutex>lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mSource = source;
    }

    // Function Execution =======================================================

    bool
    ScriptRuntime::executeOnUpdate
    (EntityRuntime* entity)
    {
        const unique_lock<mutex>lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        ScriptComponent* scriptComponent = mProjectRuntime->getScriptComponent();
		return scriptComponent->executeScriptOnUpdate(this, entity);
    }

    bool
    ScriptRuntime::executeOnInit
    (EntityRuntime* entity)
    {
        const unique_lock<mutex>lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        ScriptComponent* scriptComponent = mProjectRuntime->getScriptComponent();
		return scriptComponent->executeScriptOnInit(this, entity);
    }

    bool
    ScriptRuntime::executeOnEvent
    (EntityRuntime* entity)
    {
        const unique_lock<mutex>lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        ScriptComponent* scriptComponent = mProjectRuntime->getScriptComponent();
		return scriptComponent->executeScriptOnEvent(this, entity);
    }

    bool
    ScriptRuntime::executeOnInput
    (InputComponent* inputComp, SceneRuntime* sr)
    {
        const unique_lock<mutex>lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        ScriptComponent* scriptComponent = mProjectRuntime->getScriptComponent();
		return scriptComponent->executeScriptOnInput(this, inputComp, sr);
    }


    bool ScriptRuntime::registerInputScript()
    {
        const unique_lock<mutex>lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        ScriptComponent* scriptComponent = mProjectRuntime->getScriptComponent();
		return scriptComponent->registerInputScript(this);
    }

    bool ScriptRuntime::removeInputScript()
    {
        const unique_lock<mutex>lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        ScriptComponent* scriptComponent = mProjectRuntime->getScriptComponent();
		return scriptComponent->removeInputScript(this);
    }
}
