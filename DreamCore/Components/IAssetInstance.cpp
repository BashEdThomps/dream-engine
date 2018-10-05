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

#include "IAssetDefinition.h"
#include "../Scene/SceneObject/SceneObjectRuntime.h"
#include "../Utilities/Uuid.h"

namespace Dream
{
    IAssetInstance::IAssetInstance
    (
       IAssetDefinition* definition,
       SceneObjectRuntime* runtime)
          : DreamObject("IAssetInstance"),
            mLoaded(false),
            mAbsolutePath(""),
            mDefinition(definition),
            mSceneObjectRuntime(runtime)
    {}

    IAssetInstance::~IAssetInstance
    ()
    {
    }

    string
    IAssetInstance::getName
    ()
    {
        return mDefinition->getName();
    }

    string
    IAssetInstance::getUuid
    ()
    {
        return mDefinition->getUuid();
    }

    string
    IAssetInstance::getNameAndUuidString
    ()
    {
        return "["+getName()+" : "+getUuid()+"]";
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
    IAssetInstance::getSceneObjectRuntime
    ()
    {
        return mSceneObjectRuntime;
    }

} // End of Dream
