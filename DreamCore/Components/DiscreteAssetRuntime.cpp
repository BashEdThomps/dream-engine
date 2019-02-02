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
#include "TaskManager/Task.h"
#include "../Scene/SceneObject/SceneObjectRuntime.h"
#include "../Scene/SceneRuntime.h"
#include "../Project/Project.h"
#include "../Project/ProjectRuntime.h"
#include "../Project/ProjectDirectory.h"

namespace Dream
{

    DiscreteAssetRuntime::DiscreteAssetRuntime
    (AssetDefinition* def, SceneObjectRuntime* runtime)
        : AssetRuntime (def),
          mSceneObjectRuntime(runtime),
          mUpdateTask(nullptr)
    {
        #ifdef DREAM_LOG
        setLogClassName("DiscreteAssetRuntime");
        #endif
    }

    DiscreteAssetRuntime::~DiscreteAssetRuntime()
    {
        if (mUpdateTask)
        {
            mUpdateTask->setExpired(true);
        }
    }

    string
    DiscreteAssetRuntime::getAssetFilePath
    (string fmt)
    {
        auto pDir = mSceneObjectRuntime
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
        auto pDir = mSceneObjectRuntime
                ->getSceneRuntime()
                ->getProjectRuntime()
                ->getProject()
                ->getDirectory();
        return pDir->getAssetDirectoryPath(static_cast<AssetDefinition*>(mDefinition));
    }

    SceneObjectRuntime*
    DiscreteAssetRuntime::getSceneObjectRuntime
    ()
    {
        return mSceneObjectRuntime;
    }

    void
    DiscreteAssetRuntime::setUpdateTask
    (Task* t)
    {
       mUpdateTask = t;
    }

    void
    DiscreteAssetRuntime::clearUpdateTask
    ()
    {
       mUpdateTask = nullptr;
    }
}
