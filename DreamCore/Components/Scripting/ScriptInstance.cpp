/*
 * ScriptInstance
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

namespace Dream
{

    ScriptInstance::ScriptInstance
    (
        shared_ptr<ScriptDefinition> definition,
        shared_ptr<SceneObjectRuntime> transform
    )
        : IAssetInstance(definition,transform),
          mError(false),
          mInitialised(false)
    {
        setLogClassName("ScriptInstance");
        auto log = getLog();
        log->trace( "Constructing {}", mDefinition->getNameAndUuidString() );
        return;
    }

    ScriptInstance::~ScriptInstance
    ()
    {
        auto log = getLog();
        log->trace("Destructing ", mDefinition->getNameAndUuidString() );
    }

    bool
    ScriptInstance::load
    (string projectPath)
    {
        auto log = getLog();
        mAbsolutePath = projectPath + mDefinition->getAssetPath();
        log->info( "Script at {}" , mAbsolutePath );
        return mAbsolutePath.size() != 0;
    }

    void
    ScriptInstance::update
    ()
    {

    }

    bool
    ScriptInstance::getError
    ()
    {
        return mError;
    }

    void
    ScriptInstance::setError
    (bool err)
    {
        mError = err;
    }

    bool ScriptInstance::getInitialised() const
    {
        return mInitialised;
    }

    void ScriptInstance::setInitialised(bool initialised)
    {
        mInitialised = initialised;
    }


    void ScriptInstance::loadExtraAttributes(nlohmann::json)
    {

    }

} // End of Dream
