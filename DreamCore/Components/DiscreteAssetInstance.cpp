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
#include "DiscreteAssetInstance.h"
#include "../Scene/SceneObject/SceneObjectRuntime.h"
#include "../Scene/SceneRuntime.h"

#include "../Project/Project.h"
#include "../Project/ProjectRuntime.h"
#include "../Project/ProjectDirectory.h"

namespace Dream
{

    DiscreteAssetInstance::DiscreteAssetInstance
    (AssetDefinition* def, SceneObjectRuntime* runtime)
        : AssetInstance (def),
          mSceneObjectRuntime(runtime)
    {
        setLogClassName("DiscreteAssetInstance");
    }

    DiscreteAssetInstance::~DiscreteAssetInstance()
    {

    }

    string
    DiscreteAssetInstance::getAssetFilePath
    (string fmt)
    {
        auto pDir = mSceneObjectRuntime
                ->getSceneRuntime()
                ->getProjectRuntime()
                ->getProject()
                ->getDirectory();
        return pDir->getAssetAbsolutePath(mDefinition,fmt);
    }

    string
    DiscreteAssetInstance::getAssetDirectoryPath
    ()
    {
        auto pDir = mSceneObjectRuntime
                ->getSceneRuntime()
                ->getProjectRuntime()
                ->getProject()
                ->getDirectory();
        return pDir->getAssetDirectoryPath(mDefinition);
    }

    SceneObjectRuntime*
    DiscreteAssetInstance::getSceneObjectRuntime
    ()
    {
        return mSceneObjectRuntime;
    }
}
