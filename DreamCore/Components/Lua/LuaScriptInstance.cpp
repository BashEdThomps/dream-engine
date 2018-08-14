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

#include "LuaScriptInstance.h"

#include "ScriptDefinition.h"

namespace Dream
{

    LuaScriptInstance::LuaScriptInstance
    (ScriptDefinition* definition, SceneObjectRuntime* transform)
        : IAssetInstance(definition,transform),
          ILoggable("LuaScriptInstance"),
          mError(false)
    {
        auto log = getLog();
        log->trace( "Constructing {}", mDefinitionHandle->getNameAndUuidString() );
        return;
    }

    LuaScriptInstance::~LuaScriptInstance
    ()
    {
        auto log = getLog();
        log->trace("Destructing ", mDefinitionHandle->getNameAndUuidString() );
    }

    bool
    LuaScriptInstance::load
    (string projectPath)
    {
        auto log = getLog();
        mAbsolutePath = projectPath + mDefinitionHandle->getAssetPath();
        log->info( "Script at {}" , mAbsolutePath );
        return mAbsolutePath.size() != 0;
    }

    void
    LuaScriptInstance::update
    ()
    {

    }

    bool
    LuaScriptInstance::getError
    ()
    {
        return mError;
    }

    void
    LuaScriptInstance::setError
    (bool err)
    {
        mError = err;
    }


    void LuaScriptInstance::loadExtraAttributes(nlohmann::json)
    {

    }

} // End of Dream
