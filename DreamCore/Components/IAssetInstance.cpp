/*
* AssetInstance
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

#include "IAssetInstance.h"

#include "AssetDefinition.h"
#include "../Utilities/Uuid.h"

namespace Dream
{
    IAssetInstance::IAssetInstance
    (AssetDefinition* definition, SceneObjectRuntime* runtime)
          : mDefinitionHandle(definition),
          mSceneObjectRuntimeHandle(runtime),
          mUuid(Uuid::generateUuid())
    {

    }

    IAssetInstance::~IAssetInstance
    ()
    {
    }

    string
    IAssetInstance::getName
    ()
    {
        return mDefinitionHandle->getName();
    }

    string
    IAssetInstance::getUuid
    ()
    {
        return mUuid;
    }

    string
    IAssetInstance::getNameAndUuidString
    ()
    {
        return getName() + " (" + getUuid() + ")";
    }

    string
    IAssetInstance::getAbsolutePath
    ()
    {
        return mAbsolutePath;
    }

    void
    IAssetInstance::setAbsolutePath
    (string path)
    {
        mAbsolutePath = path;
    }

    bool
    IAssetInstance::getLoadedFlag
    ()
    {
        return mLoaded;
    }

    void
    IAssetInstance::setLoadedFlag
    (bool loaded)
    {
        mLoaded = loaded;
    }

    SceneObjectRuntime*
    IAssetInstance::getSceneObjectRuntimeHandle
    ()
    {
        return mSceneObjectRuntimeHandle;
    }
} // End of Dream
