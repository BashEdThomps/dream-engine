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

#include "DiscreteAssetRuntime.h"

#include "AssetDefinition.h"

#include "Common/Logger.h"
#include "Project/Project.h"
#include "Project/ProjectRuntime.h"
#include "Components/Storage/ProjectDirectory.h"
#include "Scene/Entity/EntityRuntime.h"
#include "Scene/SceneRuntime.h"
#include "Components/Task/Task.h"

namespace octronic::dream
{

    DiscreteAssetRuntime::DiscreteAssetRuntime
    (AssetDefinition* def, EntityRuntime* runtime)
        : AssetRuntime (def),
          mEntityRuntime(runtime)
    {
    }

    DiscreteAssetRuntime::~DiscreteAssetRuntime()
    {
    }

    string
    DiscreteAssetRuntime::getAssetFilePath
    (string fmt)
    {
        auto pDir = mEntityRuntime
                ->getSceneRuntime()
                ->getProjectRuntime()
                ->getProject()
                ->getDirectory();
        return pDir->getAssetAbsolutePath(static_cast<AssetDefinition*>(mDefinition),fmt);
    }

    string
    DiscreteAssetRuntime::getAssetDirectoryPath
    ()
    {
        auto pDir = mEntityRuntime
                ->getSceneRuntime()
                ->getProjectRuntime()
                ->getProject()
                ->getDirectory();
        return pDir->getAssetDirectoryPath(static_cast<AssetDefinition*>(mDefinition));
    }

    EntityRuntime*
    DiscreteAssetRuntime::getEntityRuntime
    ()
    {
        return mEntityRuntime;
    }
}
